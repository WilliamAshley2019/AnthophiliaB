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
    currentSampleRate = sampleRate;
    currentBlockSize  = samplesPerBlock;

    for (auto& v : voices)
    {
        v.setSampleRate (sampleRate);
        v.isActive  = false;
        v.noteHeld  = false;
        v.filter.reset();
    }

    lfo1.setSampleRate (sampleRate);
    lfo2.setSampleRate (sampleRate);

    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2u };
    distToneFilter.prepare (spec);
    *distToneFilter.state = *juce::dsp::IIR::Coefficients<float>::makeFirstOrderHighPass (
        sampleRate, 1000.0f);
}

void WASPAlphaAudioProcessor::releaseResources() {}

bool WASPAlphaAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
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
            double fltVelScale = (1.0 - (double)pVelFlt) + (double)pVelFlt * (double)v.velocity;
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

            // Call filter with CMOS mode flag — preserves original t1 path when false
            float filtered = v.filter.process (waspSig, (float)cutoffFlt, pReso, (int)pFltType, pCmos);

            // --- Amp Envelope ---
            double ampEnvVal = v.ampEnv.tick ((double)pAmpA, (double)pAmpD, (double)pAmpS, (double)pAmpR);

            // Velocity -> Amp Envelope depth (default 1.0 = full velocity sensitivity, matching
            // how most synths, and the original WASP, ship out of the box)
            double ampVelScale = (1.0 - (double)pVelAmp) + (double)pVelAmp * (double)v.velocity;
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

        // Normalise voice sum
        if (activeCount > 1)
            mixOut /= (float)activeCount;

        float lSig = mixOut;
        float rSig = mixOut;

        // --- Distortion ---
        if (pDistOn > 0.5f)
        {
            lSig = softClip (lSig, pDistDrv);
            rSig = softClip (rSig, pDistDrv);
        }

        left[sample]  = lSig;
        right[sample] = rSig;
    }

    // Dist tone filter (applied per-block to output)
    if (pDistOn > 0.5f)
    {
        *distToneFilter.state = *juce::dsp::IIR::Coefficients<float>::makeFirstOrderHighPass (
            currentSampleRate, pDistTone);

        juce::dsp::AudioBlock<float>       block (buffer);
        juce::dsp::ProcessContextReplacing<float> ctx (block);
        distToneFilter.process (ctx);
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
