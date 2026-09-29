#pragma once
#include <JuceHeader.h>

//==============================================================================
// WaspFlPresetImporter
//This was an attempt to somehow get was presets into Anthophilia it is mostly untested some work may still need apply.
//testing is requiried this is a work in progress
//==============================================================================

namespace WaspFst
{
    struct Packet
    {
        juce::String id;
        juce::MemoryBlock data;
    };

    struct FLVersion
    {
        int major = 0, minor = 0, patch = 0, build = 0;

        bool operator>= (const FLVersion& other) const
        {
            if (major != other.major) return major > other.major;
            if (minor != other.minor) return minor > other.minor;
            if (patch != other.patch) return patch > other.patch;
            return build >= other.build;
        }

        juce::String toString_() const
        {
            return juce::String (major) + "." + juce::String (minor) + "."
                 + juce::String (patch) + "." + juce::String (build);
        }

        // Parses "10.0.5" or "10.0.5.1234" style strings from the Version event.
        static FLVersion parse (const juce::String& s)
        {
            FLVersion v;
            auto tokens = juce::StringArray::fromTokens (s, ".", "");
            if (tokens.size() > 0) v.major = tokens[0].getIntValue();
            if (tokens.size() > 1) v.minor = tokens[1].getIntValue();
            if (tokens.size() > 2) v.patch = tokens[2].getIntValue();
            if (tokens.size() > 3) v.build = tokens[3].getIntValue();
            return v;
        }
    };

    struct Event
    {
        int id = 0;
        juce::MemoryBlock raw; // exact bytes as stored (before any string/int decode)
    };

  
    inline juce::Array<Packet> readPackets (const void* fileData, size_t fileSize)
    {
        juce::Array<Packet> result;
        juce::MemoryInputStream in (fileData, fileSize, false);

        while (in.getNumBytesRemaining() >= 8)
        {
            char idBytes[4];
            if (in.read (idBytes, 4) < 4) break;

            Packet p;
            p.id = juce::String::createStringFromData (idBytes, 4);

            auto length = in.readInt(); // little-endian int32
            if (length < 0 || (size_t) length > (size_t) in.getNumBytesRemaining())
                break;

            p.data.setSize ((size_t) length);
            in.read (p.data.getData(), length);
            result.add (p);
        }
        return result;
    }

  
    inline int readVarInt (const uint8_t* data, size_t size, size_t& pos)
    {
        int result = 0, shift = 0;
        uint8_t b;
        do
        {
            if (pos >= size) return result; // truncated; return what we have
            b = data[pos++];
            result |= (int) (b & 0x7F) << shift;
            shift += 7;
        } while (b & 0x80);
        return result;
    }

    
    inline juce::Array<Event> readEvents (const juce::MemoryBlock& fldtPayload)
    {
        juce::Array<Event> events;
        auto* data = (const uint8_t*) fldtPayload.getData();
        size_t size = fldtPayload.getSize();
        size_t pos = 0;

        while (pos < size)
        {
            int eid = data[pos++];
            size_t valueSize;

            if (eid < 64)       valueSize = 1;
            else if (eid < 128) valueSize = 2;
            else if (eid < 192) valueSize = 4;
            else                valueSize = (size_t) readVarInt (data, size, pos);

            if (pos + valueSize > size)
                break; // truncated/corrupt — stop rather than read garbage

            Event ev;
            ev.id = eid;
            ev.raw.setSize (valueSize);
            ev.raw.copyFrom (data + pos, 0, valueSize);
            pos += valueSize;

            events.add (ev);
        }
        return events;
    }

     
    inline juce::String decodeString (const Event& ev, const FLVersion& fileVersion)
    {
        static const FLVersion kUtf16Threshold { 11, 5, 0, 0 };
        auto* bytes = (const char*) ev.raw.getData();
        auto  size  = ev.raw.getSize();

        if (fileVersion >= kUtf16Threshold)
        {
            return juce::String (juce::CharPointer_UTF16 ((const juce::CharPointer_UTF16::CharType*) bytes),
                                  size / 2);
        }
        // ASCII, null-terminated — strip the trailing null(s) if present.
        juce::String s = juce::String::fromUTF8 (bytes, (int) size);
        while (s.isNotEmpty() && s.getLastCharacter() == 0)
            s = s.dropLastCharacters (1);
        return s;
    }

    //==========================================================================
    // Convenience struct bundling what we actually care about from an .fst.
    struct WaspPreset
    {
        bool          valid = false;
        FLVersion     version;
        juce::String  pluginFactory;   // expect "Wasp"
        juce::String  presetName;      // e.g. "Pilchard"
        juce::MemoryBlock pluginParams; // the raw Wasp-specific parameter blob (event 213)
    };

    inline WaspPreset loadFromFile (const juce::File& fstFile)
    {
        WaspPreset result;

        juce::MemoryBlock fileData;
        if (! fstFile.loadFileAsData (fileData))
        {
            DBG ("WaspFst: couldn't read " << fstFile.getFullPathName());
            return result;
        }

        auto packets = readPackets (fileData.getData(), fileData.getSize());
        const Packet* fldt = nullptr;
        for (auto& p : packets)
            if (p.id == "FLdt") { fldt = &p; break; }

        if (fldt == nullptr)
        {
            DBG ("WaspFst: no FLdt packet found in " << fstFile.getFullPathName());
            return result;
        }

        auto events = readEvents (fldt->data);

        // Version must be read first since string decoding depends on it.
        for (auto& ev : events)
        {
            if (ev.id == 199) // Version
            {
                // Version itself is version-independent (always readable as
                // ASCII per real-world captures so far) — read raw ASCII here.
                juce::String verStr = juce::String::fromUTF8 ((const char*) ev.raw.getData(), (int) ev.raw.getSize());
                while (verStr.isNotEmpty() && verStr.getLastCharacter() == 0)
                    verStr = verStr.dropLastCharacters (1);
                result.version = FLVersion::parse (verStr);
                break;
            }
        }

        for (auto& ev : events)
        {
            switch (ev.id)
            {
                case 201: // PluginFactory
                    result.pluginFactory = decodeString (ev, result.version);
                    break;
                case 203: // PluginName
                    result.presetName = decodeString (ev, result.version);
                    break;
                case 213: // PluginParams — the actual Wasp knob/switch data
                    result.pluginParams = ev.raw;
                    break;
                default:
                    break;
            }
        }

        result.valid = result.pluginParams.getSize() > 0;
        return result;
    }

       inline std::vector<uint32_t> readParamsAsU32 (const juce::MemoryBlock& pluginParams, size_t* leftoverBytes = nullptr)
    {
        std::vector<uint32_t> values;
        auto* bytes = (const uint8_t*) pluginParams.getData();
        size_t size = pluginParams.getSize();
        size_t n = size / 4;

        for (size_t i = 0; i < n; ++i)
            values.push_back ((uint32_t) juce::ByteOrder::littleEndianInt (bytes + i * 4));

        if (leftoverBytes != nullptr)
            *leftoverBytes = size - n * 4;

        return values;
    }

   
     inline juce::String dumpParamsForDiff (const WaspPreset& preset)
    {
        if (! preset.valid) return "invalid preset";

        juce::String out;
        out << "version=" << preset.version.toString_() << " factory=" << preset.pluginFactory
            << " name=" << preset.presetName << "\n";

        size_t leftover = 0;
        auto values = readParamsAsU32 (preset.pluginParams, &leftover);
        for (size_t i = 0; i < values.size(); ++i)
            out << "  [" << (int) i << "] = " << (int) values[i] << "\n";
        if (leftover > 0)
            out << "  (+" << (int) leftover << " trailing byte(s), not shown)\n";

        return out;
    }
}
