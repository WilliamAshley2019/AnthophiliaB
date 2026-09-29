#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// Parameter Layout
//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout WASPAlphaAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    using namespace WaspParams;

    // Osc 1
    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { OSC1_SHAPE, 1 }, "Osc-1 Shape",
        juce::StringArray { "Saw", "Square", "Triangle", "Sine" }, 0));
    layout.add (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID { OSC1_COARSE, 1 }, "Osc-1 Coarse", -24, 24, 0));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { OSC1_FINE, 1 }, "Osc-1 Fine",
        juce::NormalisableRange<float> (-100.0f, 100.0f, 1.0f), 0.0f));

    // Osc 2
    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { OSC2_SHAPE, 1 }, "Osc-2 Shape",
        juce::StringArray { "Saw", "Square", "Triangle", "Sine" }, 0));
    layout.add (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID { OSC2_COARSE, 1 }, "Osc-2 Coarse", -24, 24, 0));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { OSC2_FINE, 1 }, "Osc-2 Fine",
        juce::NormalisableRange<float> (-100.0f, 100.0f, 1.0f), 0.0f));

    // Osc 3 (sub/noise)
    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { OSC3_SHAPE, 1 }, "Osc-3 Shape",
        juce::StringArray { "Sub", "Sub-Sub", "Noise" }, 0));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { OSC3_AMOUNT, 1 }, "Osc-3 Amount",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.0f));

    // Mix / Mod
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { OSC_MIX, 1 }, "Osc Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.5f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PULSE_WIDTH, 1 }, "Pulse Width",
        juce::NormalisableRange<float> (0.05f, 0.95f, 0.001f), 0.5f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { FM_AMOUNT, 1 }, "FM",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { RM_AMOUNT, 1 }, "RM",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.0f));

    // Amp Envelope
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { AMP_ATTACK, 1 }, "Amp Attack",
        juce::NormalisableRange<float> (0.001f, 8.0f, 0.001f, 0.4f), 0.01f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { AMP_DECAY, 1 }, "Amp Decay",
        juce::NormalisableRange<float> (0.001f, 8.0f, 0.001f, 0.4f), 0.1f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { AMP_SUSTAIN, 1 }, "Amp Sustain",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.7f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { AMP_RELEASE, 1 }, "Amp Release",
        juce::NormalisableRange<float> (0.001f, 8.0f, 0.001f, 0.4f), 0.3f));

    // Filter Envelope
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { FLT_ATTACK, 1 }, "Filter Attack",
        juce::NormalisableRange<float> (0.001f, 8.0f, 0.001f, 0.4f), 0.01f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { FLT_DECAY, 1 }, "Filter Decay",
        juce::NormalisableRange<float> (0.001f, 8.0f, 0.001f, 0.4f), 0.3f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { FLT_SUSTAIN, 1 }, "Filter Sustain",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.5f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { FLT_RELEASE, 1 }, "Filter Release",
        juce::NormalisableRange<float> (0.001f, 8.0f, 0.001f, 0.4f), 0.5f));

    // Filter
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { FLT_KEYTRACK, 1 }, "Filter Keytrack",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.0f));
    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { FLT_TYPE, 1 }, "Filter Type",
        juce::StringArray { "LP12", "LP24", "HP12", "BP12" }, 0));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { FLT_CUTOFF, 1 }, "Cutoff",
        juce::NormalisableRange<float> (20.0f, 18000.0f, 0.1f, 0.3f), 2000.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { FLT_RESO, 1 }, "Resonance",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { FLT_ENV_AMT, 1 }, "Envelope Amount",
        juce::NormalisableRange<float> (-1.0f, 1.0f, 0.001f), 0.5f));

    // Filter 2 — dual multimode resonant filter (KillerBee)
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { FLT2_KEYTRACK, 1 }, "Filter 2 Keytrack",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.0f));
    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { FLT2_TYPE, 1 }, "Filter 2 Type",
        juce::StringArray { "LP12", "LP24", "HP12", "BP12" }, 2)); // default HP12 so series mode is useful out of the box
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { FLT2_CUTOFF, 1 }, "Cutoff 2",
        juce::NormalisableRange<float> (20.0f, 18000.0f, 0.1f, 0.3f), 4000.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { FLT2_RESO, 1 }, "Resonance 2",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { FLT2_ENV_AMT, 1 }, "Envelope Amount 2",
        juce::NormalisableRange<float> (-1.0f, 1.0f, 0.001f), 0.0f));
    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { FLT_ROUTING, 1 }, "Filter Routing",
        juce::StringArray { "Filter 1 Only", "Filter 2 Only", "Series (1->2)", "Parallel" }, 0));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { FLT_PARALLEL_MIX, 1 }, "Filter Parallel Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.5f)); // 0 = all Filter1, 1 = all Filter2

    // LFO 1
    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { LFO1_SHAPE, 1 }, "Lfo-1 Shape",
        juce::StringArray { "Sine", "Triangle", "Saw", "RevSaw", "Square", "S&H" }, 0));
    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { LFO1_TARGET, 1 }, "Lfo-1 Target",
        juce::StringArray { "Pitch", "Osc1 Pitch", "Osc2 Pitch", "Pulse Width", "Filter", "Amp" }, 0));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { LFO1_AMOUNT, 1 }, "Lfo-1 Amount",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { LFO1_FREQ, 1 }, "Lfo-1 Freq",
        juce::NormalisableRange<float> (0.01f, 20.0f, 0.001f, 0.4f), 2.0f));
    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { LFO1_SYNC, 1 }, "Lfo-1 Sync", false));
    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { LFO1_RESET, 1 }, "Lfo-1 Reset", false));

    // LFO 2
    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { LFO2_SHAPE, 1 }, "Lfo-2 Shape",
        juce::StringArray { "Sine", "Triangle", "Saw", "RevSaw", "Square", "S&H" }, 0));
    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { LFO2_TARGET, 1 }, "Lfo-2 Target",
        juce::StringArray { "Pitch", "Osc1 Pitch", "Osc2 Pitch", "Pulse Width", "Filter", "Amp" }, 0));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { LFO2_AMOUNT, 1 }, "Lfo-2 Amount",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { LFO2_FREQ, 1 }, "Lfo-2 Freq",
        juce::NormalisableRange<float> (0.01f, 20.0f, 0.001f, 0.4f), 3.0f));
    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { LFO2_SYNC, 1 }, "Lfo-2 Sync", false));
    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { LFO2_RESET, 1 }, "Lfo-2 Reset", false));

    // Distortion
    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { DIST_ON, 1 }, "Distortion On", false));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { DIST_DRIVE, 1 }, "Dist Drive",
        juce::NormalisableRange<float> (1.0f, 20.0f, 0.01f), 1.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { DIST_TONE, 1 }, "Dist Tone",
        juce::NormalisableRange<float> (200.0f, 8000.0f, 1.0f, 0.4f), 2000.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { DIST_MIX, 1 }, "Dist Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 1.0f)); // 1.0 = fully wet, matches old (only) behaviour

    // Character
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { DUAL_VOICE, 1 }, "Dual Voice",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { WASP_CHAR, 1 }, "Wasp",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { VCO_BINARY, 1 }, "VCO Binary",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.0f));

    // CMOS filter mode (authentic EDP WASP transmission-gate filter emulation)
    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { CMOS_FILTER, 1 }, "CMOS Filter", false));

    // CV (Control Voltage) — modulates a chosen destination at audio rate from Osc1
    // This lets you use Osc1 as a true CV source into the filter or pitch (FM-style via CV path)
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { CV_AMOUNT, 1 }, "CV Amount",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.0f));
    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { CV_TARGET, 1 }, "CV Target",
        juce::StringArray { "Filter Cutoff", "Osc2 Pitch", "Pulse Width", "Amp" }, 0));

    // Mod Env (one-shot AD generator, EDP WASP MOD ENV section)
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { MODENV_ATTACK, 1 }, "Mod Env Attack",
        juce::NormalisableRange<float> (0.001f, 4.0f, 0.001f, 0.4f), 0.01f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { MODENV_DECAY, 1 }, "Mod Env Decay",
        juce::NormalisableRange<float> (0.001f, 4.0f, 0.001f, 0.4f), 0.2f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { MODENV_AMOUNT, 1 }, "Mod Env Amount",
        juce::NormalisableRange<float> (-1.0f, 1.0f, 0.001f), 0.0f));
    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { MODENV_DEST, 1 }, "Mod Env Destination",
        juce::StringArray { "Pulse Width", "LFO1 Amount", "Osc1 Level", "Osc2 Pitch" }, 0));

    // Velocity (original WASP / XT VELOCITY section)
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { VEL_TO_AMP, 1 }, "Velocity to Amp",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 1.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { VEL_TO_FILTER, 1 }, "Velocity to Filter",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.0f));

    // LFO onset delay
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { LFO1_DELAY, 1 }, "Lfo-1 Delay",
        juce::NormalisableRange<float> (0.0f, 4.0f, 0.001f, 0.5f), 0.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { LFO2_DELAY, 1 }, "Lfo-2 Delay",
        juce::NormalisableRange<float> (0.0f, 4.0f, 0.001f, 0.5f), 0.0f));

    // True unison detune depth for Dual Voice (cents)
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { DUAL_DETUNE, 1 }, "Dual Voice Detune",
        juce::NormalisableRange<float> (0.0f, 50.0f, 0.1f), 8.0f));

    // Delay (KillerBee addition) — simple single-tap feedback delay.
    // Time uses a skew so the usable 50ms-1s "human" range gets most of the knob travel,
    // rather than being crammed into the first few degrees of rotation.
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { DELAY_TIME_MS, 1 }, "Delay Time",
        juce::NormalisableRange<float> (1.0f, 2000.0f, 0.1f, 0.35f), 350.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { DELAY_FEEDBACK, 1 }, "Delay Feedback",
        juce::NormalisableRange<float> (0.0f, 0.95f, 0.001f), 0.3f)); // capped below 1.0 so it can't runaway
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { DELAY_MIX, 1 }, "Delay Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.0f)); // 0 = off by default, opt-in

    // Master Volume — was missing entirely; this is the main practical fix for
    // "everything gets quiet" since there was previously no way to compensate.
    // Range and default chosen so unity (0 dB) sits at a sensible knob position,
    // not buried at one extreme.
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { MASTER_VOLUME, 1 }, "Master Volume",
        juce::NormalisableRange<float> (-60.0f, 6.0f, 0.01f, 3.0f), 0.0f));

    return layout;
}

//==============================================================================
WASPAlphaAudioProcessor::WASPAlphaAudioProcessor()
    : AudioProcessor (BusesProperties()
                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "WASPAlphaState", createParameterLayout())
{
    using namespace WaspParams;
    // Register listener for all params
    for (auto* param : apvts.processor.getParameters())
    {
        if (auto* p = dynamic_cast<juce::RangedAudioParameter*> (param))
            apvts.addParameterListener (p->getParameterID(), this);
    }
}

WASPAlphaAudioProcessor::~WASPAlphaAudioProcessor()
{
    for (auto* param : apvts.processor.getParameters())
    {
        if (auto* p = dynamic_cast<juce::RangedAudioParameter*> (param))
            apvts.removeParameterListener (p->getParameterID(), this);
    }
}

//==============================================================================
void WASPAlphaAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Defensive clamp: guards every calculation below (and inside prepare()
    // calls for voices/delay/etc) against a placeholder/invalid value some
    // hosts pass during an initial validation pass, before real audio is
    // selected. See SimpleDelay::prepare() for the specific crash this fixes.
    sampleRate     = juce::jlimit (8000.0, 384000.0, sampleRate);
    samplesPerBlock = juce::jmax (1, samplesPerBlock);

    currentSampleRate = sampleRate;
    currentBlockSize  = samplesPerBlock;

    for (auto& v : voices)
    {
        v.setSampleRate (sampleRate);
        v.isActive  = false;
        v.noteHeld  = false;
        v.filter.reset();
        v.filter2.reset();
    }

    lfo1.setSampleRate (sampleRate);
    lfo2.setSampleRate (sampleRate);

    delayL.prepare (sampleRate);
    delayR.prepare (sampleRate);

    distDryL.assign ((size_t) samplesPerBlock, 0.0f);
    distDryR.assign ((size_t) samplesPerBlock, 0.0f);

    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2u };
    distToneFilter.prepare (spec);
    *distToneFilter.state = *juce::dsp::IIR::Coefficients<float>::makeFirstOrderHighPass (
        sampleRate, 1000.0f);
    lastDistTone = 1000.0f; // matches the initial coefficients just set, so the
                            // first real block doesn't recompute for nothing

    isPrepared = true;
}

void WASPAlphaAudioProcessor::releaseResources()
{
    // Host must call prepareToPlay again before processBlock is safe to run —
    // matches the isPrepared guard added to processBlock.
    isPrepared = false;
}

bool WASPAlphaAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // We never declare an input bus in BusesProperties (see the constructor),
    // so getMainInputChannelSet() is already always disabled() for any layout
    // a host could negotiate — this check can't change behaviour, it's just
    // an explicit, self-documenting statement of intent for a VST3 instrument.
    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::disabled())
        return false;

    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

//==============================================================================
void WASPAlphaAudioProcessor::parameterChanged (const juce::String& paramID, float newValue)
{
    juce::ignoreUnused (paramID, newValue);
}

//==============================================================================
int WASPAlphaAudioProcessor::findFreeVoice()
{
    // 1. Prefer a fully silent voice
    for (int i = 0; i < MAX_VOICES; ++i)
        if (!voices[i].isActive) return i;

    // 2. Steal the quietest voice that is in release (noteHeld == false)
    int    stealIdx = -1;
    double quietest = 1.0e9;
    for (int i = 0; i < MAX_VOICES; ++i)
    {
        if (!voices[i].noteHeld && voices[i].ampEnv.value < quietest)
        {
            quietest = voices[i].ampEnv.value;
            stealIdx = i;
        }
    }
    if (stealIdx >= 0) return stealIdx;

    // 3. Last resort: steal the quietest active voice regardless
    stealIdx = 0;
    quietest = voices[0].ampEnv.value;
    for (int i = 1; i < MAX_VOICES; ++i)
    {
        if (voices[i].ampEnv.value < quietest)
        {
            quietest = voices[i].ampEnv.value;
            stealIdx = i;
        }
    }
    return stealIdx;
}

int WASPAlphaAudioProcessor::findVoiceForNote (int midiNote)
{
    // Match on noteHeld so we can find voices still in release phase
    for (int i = 0; i < MAX_VOICES; ++i)
        if (voices[i].noteHeld && voices[i].midiNote == midiNote) return i;
    return -1;
}

void WASPAlphaAudioProcessor::handleMidiEvent (const juce::MidiMessage& msg)
{
    if (msg.isNoteOn())
    {
        int note   = msg.getNoteNumber();
        double freq = juce::MidiMessage::getMidiNoteInHertz (note);

        // LFO delay always retriggers on a new note; phase only resets if RESET is engaged
        lfo1.resetDelay();
        lfo2.resetDelay();
        if (apvts.getRawParameterValue (WaspParams::LFO1_RESET)->load() > 0.5f)
            lfo1.reset();
        if (apvts.getRawParameterValue (WaspParams::LFO2_RESET)->load() > 0.5f)
            lfo2.reset();

        float vel = juce::jlimit (0.0f, 1.0f, msg.getFloatVelocity());
        int vi = findFreeVoice();
        voices[vi].noteOn (note, freq, vel);
        voices[vi].filter.reset();
        voices[vi].filter2.reset();
    }
    else if (msg.isNoteOff())
    {
        int vi = findVoiceForNote (msg.getNoteNumber());
        if (vi >= 0) voices[vi].noteOff();
    }
    else if (msg.isAllNotesOff() || msg.isAllSoundOff())
    {
        for (auto& v : voices) { v.noteOff(); v.isActive = false; v.noteHeld = false; }
    }
}

//==============================================================================
// Helper: midi note to Hz with semitone/cent offset
static double midiToHz (int note, int coarse, float fineCents)
{
    double totalCents = (double)(note + coarse) * 100.0 + (double)fineCents;
    return 440.0 * std::pow (2.0, (totalCents - 6900.0) / 1200.0);
}

// Soft clip (tanh approx) for Wasp character
static float softClip (float x, float drive)
{
    float driven = x * drive;
    // Fast tanh approximation
    float ax = std::abs (driven);
    float z  = driven * (27.0f + ax * ax) / (27.0f + 9.0f * ax * ax);
    return z / drive;
}

//==============================================================================
void WASPAlphaAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                             juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    // Some hosts' plugin validation passes call processBlock before ever
    // calling prepareToPlay. distToneFilter has undefined behaviour if
    // .process() runs before .prepare() ever ran on it — bail out silently
    // (buffer.clear() above already ran, so output is just silence) rather
    // than touch anything that assumes prepareToPlay has happened.
    if (! isPrepared)
        return;

    // --- Collect all parameters once per block ---
    using namespace WaspParams;

    float pOsc1Shape  = apvts.getRawParameterValue (OSC1_SHAPE)->load();
    float pOsc1Coarse = apvts.getRawParameterValue (OSC1_COARSE)->load();
    float pOsc1Fine   = apvts.getRawParameterValue (OSC1_FINE)->load();
    float pOsc2Shape  = apvts.getRawParameterValue (OSC2_SHAPE)->load();
    float pOsc2Coarse = apvts.getRawParameterValue (OSC2_COARSE)->load();
    float pOsc2Fine   = apvts.getRawParameterValue (OSC2_FINE)->load();
    float pOsc3Shape  = apvts.getRawParameterValue (OSC3_SHAPE)->load();
    float pOsc3Amt    = apvts.getRawParameterValue (OSC3_AMOUNT)->load();
    float pOscMix     = apvts.getRawParameterValue (OSC_MIX)->load();
    float pPW         = apvts.getRawParameterValue (PULSE_WIDTH)->load();
    float pFM         = apvts.getRawParameterValue (FM_AMOUNT)->load();
    float pRM         = apvts.getRawParameterValue (RM_AMOUNT)->load();

    float pAmpA   = apvts.getRawParameterValue (AMP_ATTACK)->load();
    float pAmpD   = apvts.getRawParameterValue (AMP_DECAY)->load();
    float pAmpS   = apvts.getRawParameterValue (AMP_SUSTAIN)->load();
    float pAmpR   = apvts.getRawParameterValue (AMP_RELEASE)->load();
    float pFltA   = apvts.getRawParameterValue (FLT_ATTACK)->load();
    float pFltD   = apvts.getRawParameterValue (FLT_DECAY)->load();
    float pFltS   = apvts.getRawParameterValue (FLT_SUSTAIN)->load();
    float pFltR   = apvts.getRawParameterValue (FLT_RELEASE)->load();

    float pFltKT  = apvts.getRawParameterValue (FLT_KEYTRACK)->load();
    float pFltType = apvts.getRawParameterValue (FLT_TYPE)->load();
    float pCutoff = apvts.getRawParameterValue (FLT_CUTOFF)->load();
    float pReso   = apvts.getRawParameterValue (FLT_RESO)->load();
    float pFltEnv = apvts.getRawParameterValue (FLT_ENV_AMT)->load();

    float pFlt2KT   = apvts.getRawParameterValue (FLT2_KEYTRACK)->load();
    float pFlt2Type = apvts.getRawParameterValue (FLT2_TYPE)->load();
    float pCutoff2  = apvts.getRawParameterValue (FLT2_CUTOFF)->load();
    float pReso2    = apvts.getRawParameterValue (FLT2_RESO)->load();
    float pFlt2Env  = apvts.getRawParameterValue (FLT2_ENV_AMT)->load();
    int   pFltRoute = (int) apvts.getRawParameterValue (FLT_ROUTING)->load();
    float pFltParMix = apvts.getRawParameterValue (FLT_PARALLEL_MIX)->load();

    float pDelayTimeMs = apvts.getRawParameterValue (DELAY_TIME_MS)->load();
    float pDelayFb     = apvts.getRawParameterValue (DELAY_FEEDBACK)->load();
    float pDelayMix    = apvts.getRawParameterValue (DELAY_MIX)->load();
    float pMasterVolDb = apvts.getRawParameterValue (MASTER_VOLUME)->load();
    float pMasterGain  = juce::Decibels::decibelsToGain (pMasterVolDb, -60.0f);

    float pL1Shape  = apvts.getRawParameterValue (LFO1_SHAPE)->load();
    float pL1Target = apvts.getRawParameterValue (LFO1_TARGET)->load();
    float pL1Amt    = apvts.getRawParameterValue (LFO1_AMOUNT)->load();
    float pL1Freq   = apvts.getRawParameterValue (LFO1_FREQ)->load();

    float pL2Shape  = apvts.getRawParameterValue (LFO2_SHAPE)->load();
    float pL2Target = apvts.getRawParameterValue (LFO2_TARGET)->load();
    float pL2Amt    = apvts.getRawParameterValue (LFO2_AMOUNT)->load();
    float pL2Freq   = apvts.getRawParameterValue (LFO2_FREQ)->load();

    float pDistOn   = apvts.getRawParameterValue (DIST_ON)->load();
    float pDistDrv  = apvts.getRawParameterValue (DIST_DRIVE)->load();
    float pDistTone = apvts.getRawParameterValue (DIST_TONE)->load();
    float pDistMix  = apvts.getRawParameterValue (DIST_MIX)->load();
    float pDualV    = apvts.getRawParameterValue (DUAL_VOICE)->load();
    float pWasp     = apvts.getRawParameterValue (WASP_CHAR)->load();
    float pVCOBin   = apvts.getRawParameterValue (VCO_BINARY)->load();
    bool  pCmos     = apvts.getRawParameterValue (CMOS_FILTER)->load() > 0.5f;
    float pCVAmt    = apvts.getRawParameterValue (CV_AMOUNT)->load();
    int   pCVTarget = (int)apvts.getRawParameterValue (CV_TARGET)->load();

    float pModEnvA    = apvts.getRawParameterValue (MODENV_ATTACK)->load();
    float pModEnvD    = apvts.getRawParameterValue (MODENV_DECAY)->load();
    float pModEnvAmt  = apvts.getRawParameterValue (MODENV_AMOUNT)->load();
    int   pModEnvDest = (int)apvts.getRawParameterValue (MODENV_DEST)->load();

    float pVelAmp     = apvts.getRawParameterValue (VEL_TO_AMP)->load();
    float pVelFlt     = apvts.getRawParameterValue (VEL_TO_FILTER)->load();

    float pLfo1Delay  = apvts.getRawParameterValue (LFO1_DELAY)->load();
    float pLfo2Delay  = apvts.getRawParameterValue (LFO2_DELAY)->load();
    float pDualDetune = apvts.getRawParameterValue (DUAL_DETUNE)->load();

    // Defensive channel-count guard. isBusesLayoutSupported only accepts an
    // exact stereo layout, so this should never trigger in normal use — but
    // some hosts' plugin validation passes probe processBlock with buffers
    // that don't match the negotiated layout before real audio settles in.
    // buffer.getWritePointer(1) below is an out-of-bounds write on anything
    // less than 2 channels; that's a real crash risk that was previously
    // masked by the prepareToPlay sample-rate bug crashing first — now that
    // that one's fixed, this is the next thing worth guarding defensively.
    if (buffer.getNumChannels() != 2)
        return; // buffer.clear() above already ran, so output stays silent rather than crashing

    float* left  = buffer.getWritePointer (0);
    float* right = buffer.getWritePointer (1);
    int    numSamples = buffer.getNumSamples();

    // MIDI event iterator
    auto midiIt = midiMessages.begin();

    for (int sample = 0; sample < numSamples; ++sample)
    {
        // Process MIDI events scheduled up to this sample
        while (midiIt != midiMessages.end())
        {
            auto meta = *midiIt;
            if (meta.samplePosition > sample) break;
            handleMidiEvent (meta.getMessage());
            ++midiIt;
        }

        // Tick LFOs (delay gain fades the LFO in after its onset delay elapses)
        double lfo1Gain = lfo1.delayGain ((double)pLfo1Delay);
        double lfo2Gain = lfo2.delayGain ((double)pLfo2Delay);
        double lfo1Val = lfo1.tick ((double)pL1Freq, (int)pL1Shape) * (double)pL1Amt * lfo1Gain;
        double lfo2Val = lfo2.tick ((double)pL2Freq, (int)pL2Shape) * (double)pL2Amt * lfo2Gain;

        float mixOut = 0.0f;
        int   activeCount = 0;

        for (auto& v : voices)
        {
            // Skip voices that are fully silent (neither active nor in release tail)
            if (!v.isActive) continue;

            // --- Frequencies ---
            double f1 = midiToHz (v.midiNote, (int)pOsc1Coarse, pOsc1Fine);
            double f2 = midiToHz (v.midiNote, (int)pOsc2Coarse, pOsc2Fine);

            // --- Mod Env (one-shot AD, EDP WASP MOD ENV section) ---
            double modEnvRaw = v.modEnv.tick ((double)pModEnvA, (double)pModEnvD);
            double modEnvSig = modEnvRaw * (double)pModEnvAmt; // signed, -1..1 scaled by amount knob

            // Per-voice local copies of the shared LFO values, so Mod Env can scale
            // "LFO1 AMT" independently for each currently-sounding voice.
            double lfo1ValLocal = lfo1Val;
            double lfo2ValLocal = lfo2Val;
            if (pModEnvDest == 1) // LFO1 Amount
                lfo1ValLocal *= juce::jlimit (0.0, 2.0, 1.0 + modEnvSig);

            // Osc2 Pitch mod-env destination (applied directly to f2 below alongside LFO pitch mod)
            if (pModEnvDest == 3)
                f2 *= std::pow (2.0, modEnvSig * 12.0 / 12.0);

            // LFO pitch modulation
            double pitchLfo1 = 0.0, pitchLfo2 = 0.0;
            if ((int)pL1Target == 0) { pitchLfo1 = lfo1ValLocal; }
            if ((int)pL1Target == 1) { f1 *= std::pow (2.0, lfo1ValLocal * 2.0 / 12.0); }
            if ((int)pL1Target == 2) { f2 *= std::pow (2.0, lfo1ValLocal * 2.0 / 12.0); }
            if ((int)pL2Target == 0) { pitchLfo2 = lfo2ValLocal; }
            if ((int)pL2Target == 1) { f1 *= std::pow (2.0, lfo2ValLocal * 2.0 / 12.0); }
            if ((int)pL2Target == 2) { f2 *= std::pow (2.0, lfo2ValLocal * 2.0 / 12.0); }

            f1 *= std::pow (2.0, (pitchLfo1 + pitchLfo2) * 2.0 / 12.0);
            f2 *= std::pow (2.0, (pitchLfo1 + pitchLfo2) * 2.0 / 12.0);

            // VCO Binary: quantises pitch to binary-divider steps (4 steps per octave)
            if (pVCOBin > 0.0f)
            {
                double stepsOctave = 4.0;
                auto quantize = [&](double freq)
                {
                    double midiF = 12.0 * std::log2 (freq / 440.0) + 69.0;
                    double step  = std::round (midiF * stepsOctave / 12.0);
                    double qMidi = step * 12.0 / stepsOctave;
                    return 440.0 * std::pow (2.0, (qMidi - 69.0) / 12.0);
                };
                double qf1 = quantize (f1);
                double qf2 = quantize (f2);
                f1 = f1 + pVCOBin * (qf1 - f1);
                f2 = f2 + pVCOBin * (qf2 - f2);
            }

            // PW modulation from LFO + Mod Env
            double pw = (double)pPW;
            if ((int)pL1Target == 3) pw = juce::jlimit (0.05, 0.95, pw + lfo1ValLocal * 0.4);
            if ((int)pL2Target == 3) pw = juce::jlimit (0.05, 0.95, pw + lfo2ValLocal * 0.4);
            if (pModEnvDest == 0) pw = juce::jlimit (0.05, 0.95, pw + modEnvSig * 0.4);

            // --- Oscillators ---
            // FM: osc1 modulates osc2 frequency
            float osc1Raw = v.oscSample (v.osc1Phase, f1, (int)pOsc1Shape, pw);

            // CV: use Osc1 output as a control voltage into chosen destination
            // This emulates patching the WASP's CV input jack — Osc1 drives Osc2 pitch,
            // filter cutoff, PW, or amp at audio rate for complex timbres.
            double cvSig = (double)osc1Raw * (double)pCVAmt;
            double f2CV = f2;
            double pwCV = pw;
            double cvCutoffMult = 1.0;
            double cvAmpMod = 1.0;
            if (pCVAmt > 0.0f)
            {
                switch (pCVTarget)
                {
                    case 0: // Filter Cutoff — CV sweeps filter ±3 octaves
                        cvCutoffMult = std::pow (2.0, cvSig * 3.0);
                        break;
                    case 1: // Osc2 Pitch — classic hard sync / FM alternative
                        f2CV *= std::pow (2.0, cvSig * 2.0 / 12.0);
                        break;
                    case 2: // Pulse Width
                        pwCV = juce::jlimit (0.05, 0.95, pw + cvSig * 0.4);
                        break;
                    case 3: // Amp — tremolo / AM from Osc1
                        cvAmpMod = juce::jlimit (0.0, 1.0, 0.5 + cvSig * 0.5);
                        break;
                    default: break;
                }
            }

            // Mod Env -> Osc1 Level destination
            if (pModEnvDest == 2)
                osc1Raw *= (float) juce::jlimit (0.0, 1.0, 1.0 + modEnvSig);

            double fmMod  = 1.0 + (double)pFM * (double)osc1Raw * 4.0;
            float osc2Raw = v.oscSample (v.osc2Phase, f2CV * fmMod, (int)pOsc2Shape, pwCV);

            // RM: ring mod of osc1 * osc2
            float rmSig  = osc1Raw * osc2Raw;
            float osc12  = osc1Raw * (1.0f - pOscMix) + osc2Raw * pOscMix;
            osc12        = osc12 * (1.0f - pRM) + rmSig * pRM;

            // Dual Voice: true unison — a second detuned oscillator pair, summed in,
            // replacing the old fake stereo-width hack. DUAL_VOICE (0..1) is the blend
            // of the detuned layer, DUAL_DETUNE (cents) is how far it's detuned.
            if (pDualV > 0.0f)
            {
                double detuneMult = std::pow (2.0, (double)pDualDetune / 1200.0);
                float osc1B = v.oscSample (v.osc1PhaseB, f1 * detuneMult, (int)pOsc1Shape, pw);
                float osc2B = v.oscSample (v.osc2PhaseB, f2CV * detuneMult * fmMod, (int)pOsc2Shape, pwCV);
                float osc12B = osc1B * (1.0f - pOscMix) + osc2B * pOscMix;
                osc12 = osc12 * (1.0f - 0.5f * pDualV) + osc12B * (0.5f * pDualV);
            }

            // Osc3 (sub / noise)
            float osc3Raw = v.osc3Sample (v.osc3Phase, v.baseFreq, (int)pOsc3Shape);
            float osc3Mix = osc12 * (1.0f - pOsc3Amt) + osc3Raw * pOsc3Amt;

            // Wasp character: quantised CMOS-divider grit
            float waspSig = osc3Mix;
            if (pWasp > 0.0f)
            {
                float quant = std::round (osc3Mix * 8.0f) / 8.0f;
                waspSig     = osc3Mix * (1.0f - pWasp) + quant * pWasp;
            }

            // --- Filter ---
            double fltEnvVal = v.fltEnv.tick ((double)pFltA, (double)pFltD, (double)pFltS, (double)pFltR);

            // Velocity -> Filter Envelope: at VEL_TO_FILTER = 0 velocity has no effect (env always
            // reaches full depth); at 1.0 the env's depth scales linearly with note-on velocity.
            // Same sqrt velocity curve as the amp envelope, for consistency.
            double fltVelScale = (1.0 - (double)pVelFlt) + (double)pVelFlt * std::sqrt ((double) v.velocity);
            fltEnvVal *= fltVelScale;

            // Keytrack
            double baseNote  = 60.0;
            double noteSemis = (double)v.midiNote - baseNote;
            double ktMult    = std::pow (2.0, noteSemis * (double)pFltKT / 12.0);
            double cutoffFlt = (double)pCutoff * ktMult;

            // Envelope modulation: +/- 3 octaves
            double envMod = (double)pFltEnv * fltEnvVal;
            cutoffFlt    *= std::pow (2.0, envMod * 3.0);

            // CV filter modulation
            cutoffFlt *= cvCutoffMult;

            // LFO filter mod
            if ((int)pL1Target == 4) cutoffFlt *= std::pow (2.0, lfo1ValLocal * 2.0);
            if ((int)pL2Target == 4) cutoffFlt *= std::pow (2.0, lfo2ValLocal * 2.0);

            cutoffFlt = juce::jlimit (20.0, (currentSampleRate * 0.49), cutoffFlt);

            // Filter 2 cutoff (independent keytrack + knobs, same env/CV/LFO treatment)
            double ktMult2    = std::pow (2.0, noteSemis * (double)pFlt2KT / 12.0);
            double cutoffFlt2 = (double)pCutoff2 * ktMult2;
            double envMod2 = (double)pFlt2Env * fltEnvVal;
            cutoffFlt2 *= std::pow (2.0, envMod2 * 3.0);
            cutoffFlt2 *= cvCutoffMult;
            if ((int)pL1Target == 4) cutoffFlt2 *= std::pow (2.0, lfo1ValLocal * 2.0);
            if ((int)pL2Target == 4) cutoffFlt2 *= std::pow (2.0, lfo2ValLocal * 2.0);
            cutoffFlt2 = juce::jlimit (20.0, (currentSampleRate * 0.49), cutoffFlt2);

            // Call filter(s) with CMOS mode flag — preserves original t1 path when false.
            // FLT_ROUTING: 0=Filter1 only, 1=Filter2 only, 2=Series (1->2), 3=Parallel (blended)
            float filtered;
            switch (pFltRoute)
            {
                case 1: // Filter 2 only
                    filtered = v.filter2.process (waspSig, (float)cutoffFlt2, pReso2, (int)pFlt2Type, pCmos);
                    break;
                case 2: // Series: Filter 1 into Filter 2
                {
                    float stage1 = v.filter.process (waspSig, (float)cutoffFlt, pReso, (int)pFltType, pCmos);
                    filtered = v.filter2.process (stage1, (float)cutoffFlt2, pReso2, (int)pFlt2Type, pCmos);
                    break;
                }
                case 3: // Parallel: both filters fed the same dry signal, blended
                {
                    float stage1 = v.filter.process (waspSig, (float)cutoffFlt, pReso, (int)pFltType, pCmos);
                    float stage2 = v.filter2.process (waspSig, (float)cutoffFlt2, pReso2, (int)pFlt2Type, pCmos);
                    filtered = stage1 * (1.0f - pFltParMix) + stage2 * pFltParMix;
                    break;
                }
                default: // Filter 1 only
                    filtered = v.filter.process (waspSig, (float)cutoffFlt, pReso, (int)pFltType, pCmos);
                    break;
            }

            // --- Amp Envelope ---
            double ampEnvVal = v.ampEnv.tick ((double)pAmpA, (double)pAmpD, (double)pAmpS, (double)pAmpR);

            // Velocity -> Amp Envelope depth. Uses a square-root curve rather than
            // linear: linear velocity meant a mid-strength hit (vel ~64/127) cut
            // amplitude by exactly -6dB with no shaping at all, which read as
            // "too sensitive"/"gets quiet fast". sqrt(velocity) keeps quiet hits
            // quiet but brings mid-to-high velocities up to a more usable level —
            // vel 64/127 now lands around -3dB instead of -6dB.
            double velocityCurved = std::sqrt ((double) v.velocity);
            double ampVelScale = (1.0 - (double)pVelAmp) + (double)pVelAmp * velocityCurved;
            ampEnvVal *= ampVelScale;

            // LFO + CV amp mod
            double ampMod = cvAmpMod;
            if ((int)pL1Target == 5) ampMod *= juce::jlimit (0.0, 1.0, 1.0 + lfo1ValLocal);
            if ((int)pL2Target == 5) ampMod *= juce::jlimit (0.0, 1.0, 1.0 + lfo2ValLocal);

            float voiceSig = filtered * (float)(ampEnvVal * ampMod);

            mixOut += voiceSig;
            ++activeCount;

            // Update isActive AFTER rendering this sample so the final release
            // sample is never dropped (this was the original note-off bug root cause)
            if (v.ampEnv.isActive() == false)
                v.isActive = false;
        }

        // Voice mix headroom. This used to divide by activeCount, which meant a
        // single held note played at full volume but adding a second held note
        // instantly cut BOTH by -6dB, a third cut further, etc — notes changing
        // loudness just because other notes were added/released is the "gets
        // quiet fast" behaviour that was reported. A fixed per-voice gain keeps
        // a single note's level constant regardless of polyphony; the tanh soft
        // limiter after Master Volume (below) catches the rare case where many
        // voices + high resonance stack up and would otherwise clip.
        constexpr float kVoiceGain = 0.5f;
        mixOut *= kVoiceGain;
        juce::ignoreUnused (activeCount);

        // Voice synthesis writes the clean, undistorted signal here. Distortion,
        // Delay, Master Volume, and the safety limiter are now separate passes
        // below (in that order) rather than interleaved in this loop — needed
        // so Distortion can have a proper dry/wet blend, and so its Tone
        // filtering happens where it belongs in the chain (right after the
        // waveshaper, not tacked on after Master Volume/the limiter like before).
        float lSig = mixOut;
        float rSig = mixOut;

        left[sample]  = lSig;
        right[sample] = rSig;
    }

    // --- Distortion (waveshaper + Tone filter, as one dry/wet-blended stage) ---
    if (pDistOn > 0.5f)
    {
        // Real-time-safe: resize-if-needed only touches the vector when it's
        // actually too small (never true in the normal case, since
        // prepareToPlay already sized these to samplesPerBlock).
        if ((int) distDryL.size() < numSamples) distDryL.resize ((size_t) numSamples);
        if ((int) distDryR.size() < numSamples) distDryR.resize ((size_t) numSamples);

        for (int i = 0; i < numSamples; ++i)
        {
            distDryL[(size_t) i] = left[i];
            distDryR[(size_t) i] = right[i];
            left[i]  = softClip (left[i],  pDistDrv);
            right[i] = softClip (right[i], pDistDrv);
        }

        // Only recompute coefficients (which heap-allocate internally via
        // makeFirstOrderHighPass) when the Tone knob has actually moved —
        // doing this unconditionally every block is a real-time-safety
        // issue (allocation on the audio thread) even though it's not
        // itself a crash under normal desktop conditions.
        if (! juce::approximatelyEqual (pDistTone, lastDistTone))
        {
            lastDistTone = pDistTone;
            *distToneFilter.state = *juce::dsp::IIR::Coefficients<float>::makeFirstOrderHighPass (
                currentSampleRate, pDistTone);
        }

        juce::dsp::AudioBlock<float>       block (buffer);
        juce::dsp::ProcessContextReplacing<float> ctx (block);
        distToneFilter.process (ctx);

        // Dry/wet blend — this is the fix for "only the delta gets through":
        // at Mix < 1.0, the Tone highpass (which can remove nearly all the
        // bass/fundamental at high Tone settings) no longer removes it from
        // the WHOLE signal, only from the wet portion; the dry portion keeps
        // the full-spectrum fundamental intact.
        for (int i = 0; i < numSamples; ++i)
        {
            left[i]  = distDryL[(size_t) i] * (1.0f - pDistMix) + left[i]  * pDistMix;
            right[i] = distDryR[(size_t) i] * (1.0f - pDistMix) + right[i] * pDistMix;
        }
    }

    // --- Delay (single-tap feedback, opt-in via mix — 0 by default) ---
    if (pDelayMix > 0.0001f)
    {
        for (int i = 0; i < numSamples; ++i)
        {
            left[i]  = delayL.process (left[i],  pDelayTimeMs / 1000.0f, pDelayFb, pDelayMix);
            right[i] = delayR.process (right[i], pDelayTimeMs / 1000.0f, pDelayFb, pDelayMix);
        }
    }

    // --- Master Volume + safety soft limiter ---
    // tanh is near-linear (and so inaudible) for normal signal levels, and only
    // engages as levels approach/exceed 0dBFS — a safety net for when several
    // voices, high resonance, and/or delay feedback stack up, rather than a
    // tone-shaping effect. Threshold chosen so typical single-note playing at
    // Master Volume = 0dB never touches it.
    constexpr float kLimiterThreshold = 1.6f;
    for (int i = 0; i < numSamples; ++i)
    {
        left[i]  = std::tanh ((left[i]  * pMasterGain) / kLimiterThreshold) * kLimiterThreshold;
        right[i] = std::tanh ((right[i] * pMasterGain) / kLimiterThreshold) * kLimiterThreshold;
    }
}

//==============================================================================
juce::AudioProcessorEditor* WASPAlphaAudioProcessor::createEditor()
{
    return new WASPAlphaAudioProcessorEditor (*this);
}

//==============================================================================
void WASPAlphaAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void WASPAlphaAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary (data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName (apvts.state.getType()))
        apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WASPAlphaAudioProcessor();
}
