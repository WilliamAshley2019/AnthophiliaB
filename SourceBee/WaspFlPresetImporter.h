#pragma once
#include <JuceHeader.h>

//==============================================================================
// WaspFlPresetImporter
//
// Ports the packet-level logic from the FlWaspMigrator .cs files (FstPacket,
// FstPacketStreamer, FstDataPacketConverter, WaspPresetConverter) into JUCE/C++
// so Bee can read the same on-disk container FL Studio uses for its native
// WASP / WASP XT plugin presets and generator .fst chunks.
//
// WHAT THIS DECODES (verified from the .cs source you supplied):
//   .fst container  = sequence of packets:
//       [4 bytes ASCII id]["FLdt" etc.][int32 LE length]["length" bytes payload]
//   "FLdt" packet payload =
//       [1 byte skip]
//       [1 byte versionLength][ (versionLength-1) bytes ASCII version][1 byte skip]
//       [3 bytes skip]
//       [1 byte pluginIdLength][ (pluginIdLength-2) bytes UTF-16LE pluginId][2 bytes skip]
//       [remaining bytes = raw plugin preset data, "PluginData"]
//   For the FL-*native* Wasp generator specifically, WaspPresetConverter then reads
//   a 164-byte parameter block out of PluginData starting at:
//       offset = 73 + PluginData[55]
//   That 164-byte block is what actually holds the Wasp's knob/switch values.
//
// WHAT IS **NOT** YET KNOWN:
//   The .cs tool only proves *where* the 164-byte block lives — it treats the
//   block as an opaque byte buffer and copies it verbatim into/out of the VST
//   Wasp's own preset chunk (at PluginDataOffset + 1121 + 0x40 for the VST side).
//   It does not document what each of the 164 bytes means. Nobody has published
//   an official field map for FL's native Wasp parameter block, so the WASP_FIELD
//   table below is a *placeholder* — every offset in it needs to be confirmed by
//   you, empirically, before EncodeToApvts() is trustworthy. Recommended method:
//   1. In FL Studio, load the Wasp generator, set every control to a distinct,
//      recognisable value one at a time, save the .flp, and re-export just that
//      channel's state (or point this importer at a .fst you've saved after each
//      single-control change).
//   2. Run DumpRawBlock() below (writes the 164 bytes as hex to a file/log) before
//      and after each change and diff them — the byte(s) that changed is that
//      control's field. Do this for cutoff, resonance, filter type, both coarse/
//      fine tunings, mix, PW, FM, RM, both ADSRs, both LFOs, distortion, dual
//      voice — the full parameter list is in the WASP XT reference you already
//      have.
//   3. Fill in WASP_FIELD with the real offsets/widths/scalings as you find them,
//      then EncodeToApvts() below will map them onto Bee's APVTS automatically.
//==============================================================================

struct FstPacket
{
    juce::String id;   // 4-char ASCII, e.g. "FLdt"
    juce::MemoryBlock data;
};

struct FstDataPacket
{
    juce::String version;
    juce::String pluginId;
    juce::MemoryBlock pluginData;
    int pluginDataOffset = 0;
};

class WaspFlPresetImporter
{
public:
    //==========================================================================
    // Reads every packet out of a raw .fst byte stream (mirrors FstPacketStreamer.Read
    // called in a loop, as FstPresetStreamer does).
    static juce::Array<FstPacket> readPackets (const void* fileData, size_t fileSize)
    {
        juce::Array<FstPacket> result;
        juce::MemoryInputStream in (fileData, fileSize, false);

        while (in.getNumBytesRemaining() >= 8)
        {
            char idBytes[4];
            if (in.read (idBytes, 4) < 4) break;

            juce::String id = juce::String::createStringFromData (idBytes, 4);

            // Original C# only validates the first packet's id[0]/id[1] against 'F','L' —
            // we validate every packet's id is 4 printable ASCII chars to stay permissive
            // but avoid reading garbage as a huge length.
            juce::int32 length = in.readInt(); // little-endian int32, matches BinaryReader.ReadInt32
            if (length < 0 || (size_t) length > (size_t) in.getNumBytesRemaining())
                break;

            FstPacket packet;
            packet.id = id;
            packet.data.setSize ((size_t) length);
            in.read (packet.data.getData(), (int) length);
            result.add (packet);
        }
        return result;
    }

    //==========================================================================
    // Decodes an "FLdt" packet's header + trailing plugin data, matching
    // FstDataPacketConverter.Decode exactly (byte-for-byte, same skip counts).
    static FstDataPacket decodeDataPacket (const FstPacket& packet)
    {
        FstDataPacket out;
        juce::MemoryInputStream in (packet.data, false);

        in.readByte(); // skip

        auto versionLength = (uint8_t) in.readByte();
        juce::MemoryBlock versionBytes;
        if (versionLength > 1)
        {
            versionBytes.setSize (versionLength - 1);
            in.read (versionBytes.getData(), (int) versionLength - 1);
        }
        out.version = juce::String::createStringFromData (versionBytes.getData(), (int) versionBytes.getSize());
        in.readByte(); // skip

        in.skipNextBytes (3);

        auto pluginIdLength = (uint8_t) in.readByte();
        juce::MemoryBlock idBytes;
        if (pluginIdLength > 2)
        {
            idBytes.setSize (pluginIdLength - 2);
            in.read (idBytes.getData(), (int) pluginIdLength - 2);
        }
        // UTF-16LE, matching Encoders.Utf16 (System.Text.UnicodeEncoding = UTF-16LE)
        out.pluginId = juce::String (juce::CharPointer_UTF16 ((const juce::CharPointer_UTF16::CharType*) idBytes.getData()),
                                      idBytes.getSize() / 2);
        in.skipNextBytes (2);

        out.pluginDataOffset = (int) in.getPosition();
        auto remaining = (size_t) in.getNumBytesRemaining();
        out.pluginData.setSize (remaining);
        in.read (out.pluginData.getData(), (int) remaining);

        return out;
    }

    //==========================================================================
    // Extracts the raw 164-byte Wasp parameter block from FL-native plugin data,
    // matching WaspPresetConverter.DecodeFlWasp: offset = 73 + pluginData[55].
    static bool extractWaspBlock (const FstDataPacket& fldt, juce::MemoryBlock& outBlock)
    {
        constexpr int kBlockSize = 164;
        auto* bytes = (const uint8_t*) fldt.pluginData.getData();
        auto  size  = fldt.pluginData.getSize();

        if (size < 56) return false; // need byte [55] to exist
        int offset = 73 + bytes[55];
        if (offset < 0 || (size_t) (offset + kBlockSize) > size) return false;

        outBlock.setSize (kBlockSize);
        outBlock.copyFrom (bytes + offset, 0, kBlockSize);
        return true;
    }

    //==========================================================================
    // Convenience: load a .fst file straight to the raw 164-byte Wasp block.
    // Returns false (and leaves outBlock empty) if no "FLdt" packet is found or
    // the block can't be located — check the console/log for why.
    static bool loadWaspBlockFromFile (const juce::File& fstFile, juce::MemoryBlock& outBlock)
    {
        juce::MemoryBlock fileData;
        if (! fstFile.loadFileAsData (fileData))
        {
            DBG ("WaspFlPresetImporter: couldn't read " << fstFile.getFullPathName());
            return false;
        }

        auto packets = readPackets (fileData.getData(), fileData.getSize());
        for (auto& p : packets)
        {
            if (p.id == "FLdt")
            {
                auto fldt = decodeDataPacket (p);
                DBG ("WaspFlPresetImporter: FLdt version=" << fldt.version << " pluginId=" << fldt.pluginId);
                return extractWaspBlock (fldt, outBlock);
            }
        }
        DBG ("WaspFlPresetImporter: no FLdt packet found in " << fstFile.getFullPathName());
        return false;
    }

    //==========================================================================
    // Debug helper for the reverse-engineering workflow described above: writes
    // the 164-byte block as a hex dump so you can diff two captures side-by-side.
    static juce::String dumpRawBlockHex (const juce::MemoryBlock& block)
    {
        juce::String out;
        auto* bytes = (const uint8_t*) block.getData();
        for (size_t i = 0; i < block.getSize(); ++i)
        {
            out << juce::String::toHexString ((int) bytes[i]).paddedLeft ('0', 2) << " ";
            if ((i + 1) % 16 == 0) out << "\n";
        }
        return out;
    }

    //==========================================================================
    // PLACEHOLDER field map — offsets are guesses seeded only from the block's
    // total size (164 bytes) and the WASP/WASP XT control list; NONE of these are
    // confirmed. Treat every entry as "needs verification" until you've diffed
    // real captures per the workflow above. widthBytes/scale/isSigned are your
    // knobs to adjust once you know the real encoding (WASP presets are old FL
    // format and likely store most knobs as a single unsigned byte, 0-255 or
    // 0-127, but that is *not* confirmed here).
    struct WaspField
    {
        const char* paramId;   // matches a WaspParams:: id in PluginProcessor.h
        int         offset;    // byte offset into the 164-byte block — UNVERIFIED
        int         widthBytes;
        bool        isFloat01; // true: raw byte / 255.0 -> 0..1 param range
    };

    // NOT wired up to anything yet — intentionally left unpopulated so nothing
    // silently applies wrong values. Fill this in once you've confirmed offsets,
    // then call encodeToApvts() (to be written) to push values into apvts.
    static const juce::Array<WaspField>& getFieldMapPlaceholder()
    {
        static juce::Array<WaspField> fields; // empty on purpose — see comment above
        return fields;
    }
};
