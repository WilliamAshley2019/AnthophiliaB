#pragma once
#include <JuceHeader.h>

//==============================================================================
// Anthophilia Plus- A tribute to the EDP Wasp (1978)
// AlphaAudio by William Ashley Music
//==============================================================================

//==============================================================================
// APVTS Parameter IDs
//==============================================================================
namespace WaspParams
{
    // Oscillator 1
    static constexpr const char* OSC1_SHAPE   = "osc1_shape";
    static constexpr const char* OSC1_COARSE  = "osc1_coarse";
    static constexpr const char* OSC1_FINE    = "osc1_fine";

    // Oscillator 2
    static constexpr const char* OSC2_SHAPE   = "osc2_shape";
    static constexpr const char* OSC2_COARSE  = "osc2_coarse";
    static constexpr const char* OSC2_FINE    = "osc2_fine";

    // Oscillator 3 (Sub / Noise osc)
    static constexpr const char* OSC3_SHAPE   = "osc3_shape";
    static constexpr const char* OSC3_AMOUNT  = "osc3_amount";

    // Mix / Modulation
    static constexpr const char* OSC_MIX      = "osc_mix";
    static constexpr const char* PULSE_WIDTH  = "pulse_width";
    static constexpr const char* FM_AMOUNT    = "fm_amount";
    static constexpr const char* RM_AMOUNT    = "rm_amount";

    // Amp Envelope
    static constexpr const char* AMP_ATTACK   = "amp_attack";
    static constexpr const char* AMP_DECAY    = "amp_decay";
    static constexpr const char* AMP_SUSTAIN  = "amp_sustain";
    static constexpr const char* AMP_RELEASE  = "amp_release";

    // Filter Envelope
    static constexpr const char* FLT_ATTACK   = "flt_attack";
    static constexpr const char* FLT_DECAY    = "flt_decay";
    static constexpr const char* FLT_SUSTAIN  = "flt_sustain";
    static constexpr const char* FLT_RELEASE  = "flt_release";

    // Filter
    static constexpr const char* FLT_KEYTRACK = "flt_keytrack";
    static constexpr const char* FLT_TYPE     = "flt_type";
    static constexpr const char* FLT_CUTOFF   = "flt_cutoff";
    static constexpr const char* FLT_RESO     = "flt_reso";
    static constexpr const char* FLT_ENV_AMT  = "flt_env_amt";

    // LFO 1
    static constexpr const char* LFO1_SHAPE   = "lfo1_shape";
    static constexpr const char* LFO1_TARGET  = "lfo1_target";
    static constexpr const char* LFO1_AMOUNT  = "lfo1_amount";
    static constexpr const char* LFO1_FREQ    = "lfo1_freq";
    static constexpr const char* LFO1_SYNC    = "lfo1_sync";
    static constexpr const char* LFO1_RESET   = "lfo1_reset";

    // LFO 2
    static constexpr const char* LFO2_SHAPE   = "lfo2_shape";
    static constexpr const char* LFO2_TARGET  = "lfo2_target";
    static constexpr const char* LFO2_AMOUNT  = "lfo2_amount";
    static constexpr const char* LFO2_FREQ    = "lfo2_freq";
    static constexpr const char* LFO2_SYNC    = "lfo2_sync";
    static constexpr const char* LFO2_RESET   = "lfo2_reset";

    // FX / Character
    static constexpr const char* DIST_ON      = "dist_on";
    static constexpr const char* DIST_DRIVE   = "dist_drive";
    static constexpr const char* DIST_TONE    = "dist_tone";
    static constexpr const char* DUAL_VOICE   = "dual_voice";
    static constexpr const char* WASP_CHAR    = "wasp_char";   // "Wasp" character mode
    static constexpr const char* VCO_BINARY   = "vco_binary";  // Binary divider ladder
    // CMOS filter emulation + CV
    static constexpr const char* CMOS_FILTER  = "cmos_filter"; // toggle CMOS vs Moog-style
    static constexpr const char* CV_AMOUNT    = "cv_amount";   // Control Voltage depth
    static constexpr const char* CV_TARGET    = "cv_target";   // CV destination

    // Mod Env — one-shot AD generator (EDP WASP "MOD ENV" section)
    static constexpr const char* MODENV_ATTACK = "modenv_attack";
    static constexpr const char* MODENV_DECAY  = "modenv_decay";
    static constexpr const char* MODENV_AMOUNT = "modenv_amount";
    static constexpr const char* MODENV_DEST   = "modenv_dest"; // PW / LFO1 AMT / OSC1 LEVEL / OSC2 PITCH

    // Velocity — depth of note velocity into Amp Env and Filter Env (original WASP/XT "VELOCITY" section)
    static constexpr const char* VEL_TO_AMP    = "vel_to_amp";
    static constexpr const char* VEL_TO_FILTER = "vel_to_filter";

    // LFO delay (onset delay before the LFO starts affecting its target) — present on both LFOs in the original/XT
    static constexpr const char* LFO1_DELAY   = "lfo1_delay";
    static constexpr const char* LFO2_DELAY   = "lfo2_delay";

    // True unison detune amount for Dual Voice (replaces the old fake stereo-width hack)
    static constexpr const char* DUAL_DETUNE  = "dual_detune";
}

//==============================================================================
// Simple Moog-style ladder filter (4-pole / 2-pole selectable)
struct WaspFilter
{
    double s[4] = { 0.0, 0.0, 0.0, 0.0 };
    double sampleRate = 44100.0;

    void reset() { for (auto& x : s) x = 0.0; }

    void setSampleRate (double sr) { sampleRate = sr; }

    // type: 0=LP12, 1=LP24, 2=HP12, 3=BP12
    // cmosMode: emulates CMOS transmission-gate filter (harder clipping, less resonance swell)
    float process (float input, float cutoffHz, float resonance, int type, bool cmosMode = false)
    {
        double f  = 2.0 * juce::MathConstants<double>::pi * (double)cutoffHz / sampleRate;
        f = juce::jlimit (0.001, 0.499, f);
        double k  = (double)resonance * 3.99;
        double t1 = f * 0.9892;  // original Moog-style integrator coefficient
        double x = (double)input - k * s[3];

        double y0, y1, y2, y3;

        if (cmosMode)
        {
            // CMOS transmission-gate model:
            // Each stage clips softly (CMOS switches have hard Vdd/Vss rails),
            // and the resonance feedback is attenuated (CMOS filters self-limit
            // rather than oscillating cleanly at high Q).
            // We model this as: tanh-saturated integrators + reduced feedback.
            double kCmos = (double)resonance * 1.8;  // CMOS resonance caps at ~1.8 before self-limit
            double xC    = (double)input - kCmos * s[3];
            // Hard-rail soft-clip to ±1 (CMOS Vdd/Vss behaviour)
            auto cmosClip = [](double v) -> double {
                return v / (1.0 + std::abs(v));  // fast soft clip mimicking transmission gate limiting
            };
            y0 = cmosClip (s[0] + t1 * (xC - s[0]));
            y1 = cmosClip (s[1] + t1 * (y0 - s[1]));
            y2 = cmosClip (s[2] + t1 * (y1 - s[2]));
            y3 = cmosClip (s[3] + t1 * (y2 - s[3]));
        }
        else
        {
            // Original Moog-style linear ladder
            y0 = x  * t1 + s[0] * (1.0 - t1);
            y1 = y0 * t1 + s[1] * (1.0 - t1);
            y2 = y1 * t1 + s[2] * (1.0 - t1);
            y3 = y2 * t1 + s[3] * (1.0 - t1);
        }

        s[0] = y0;  s[1] = y1;  s[2] = y2;  s[3] = y3;

        switch (type)
        {
            case 1: return (float)y3;          // LP24
            case 2: return (float)(x - y2);    // HP12
            case 3: return (float)(y1 - y3);   // BP12
            default: return (float)y1;         // LP12
        }
    }
};

//==============================================================================
// Simple ADSR
struct WaspEnv
{
    enum class Stage { Idle, Attack, Decay, Sustain, Release };
    Stage stage = Stage::Idle;
    double value = 0.0;
    double sampleRate = 44100.0;

    void setSampleRate (double sr) { sampleRate = sr; }
    void noteOn()  { stage = Stage::Attack; }
    void noteOff() { if (stage != Stage::Idle) stage = Stage::Release; }
    bool isActive() const { return stage != Stage::Idle; }

    double tick (double attack, double decay, double sustain, double release)
    {
        // Times in seconds
        switch (stage)
        {
            case Stage::Attack:
                value += 1.0 / (attack * sampleRate + 1.0);
                if (value >= 1.0) { value = 1.0; stage = Stage::Decay; }
                break;
            case Stage::Decay:
                value -= (1.0 - sustain) / (decay * sampleRate + 1.0);
                if (value <= sustain) { value = sustain; stage = Stage::Sustain; }
                break;
            case Stage::Sustain:
                value = sustain;
                break;
            case Stage::Release:
                value -= value / (release * sampleRate + 1.0);
                if (value < 0.0001) { value = 0.0; stage = Stage::Idle; }
                break;
            default: value = 0.0; break;
        }
        return value;
    }
};

//==============================================================================
// One-shot AD "Mod Env" — matches the EDP WASP MOD ENV section: rises over
// Attack, falls over Decay, then sits at 0 (does NOT sustain or loop).
// Retriggered on every note-on.
struct WaspModEnv
{
    enum class Stage { Idle, Attack, Decay };
    Stage  stage = Stage::Idle;
    double value = 0.0;
    double sampleRate = 44100.0;

    void setSampleRate (double sr) { sampleRate = sr; }
    void noteOn() { stage = Stage::Attack; value = 0.0; }

    double tick (double attack, double decay)
    {
        switch (stage)
        {
            case Stage::Attack:
                value += 1.0 / (attack * sampleRate + 1.0);
                if (value >= 1.0) { value = 1.0; stage = Stage::Decay; }
                break;
            case Stage::Decay:
                value -= 1.0 / (decay * sampleRate + 1.0);
                if (value <= 0.0) { value = 0.0; stage = Stage::Idle; }
                break;
            default:
                value = 0.0;
                break;
        }
        return value;
    }
};

//==============================================================================
// LFO
struct WaspLfo
{
    double phase     = 0.0;
    double sampleRate = 44100.0;
    double holdValue = 0.0;  // for S&H
    double delayTimer = 0.0; // seconds elapsed since last reset, for onset delay

    void setSampleRate (double sr) { sampleRate = sr; }
    void reset() { phase = 0.0; delayTimer = 0.0; }
    void resetDelay() { delayTimer = 0.0; } // delay retriggers every note regardless of the RESET (phase) toggle

    // Returns the delay envelope (0 while waiting, ramps 0->1 over ~10ms once past delayTime,
    // to avoid a click when the LFO output snaps in)
    double delayGain (double delayTimeSeconds)
    {
        delayTimer += 1.0 / sampleRate;
        if (delayTimeSeconds <= 0.0) return 1.0;
        double past = delayTimer - delayTimeSeconds;
        if (past <= 0.0) return 0.0;
        return juce::jlimit (0.0, 1.0, past * sampleRate / 480.0); // ~10ms fade-in
    }

    double tick (double freqHz, int shape)
    {
        double out = 0.0;
        switch (shape)
        {
            case 0: out = std::sin (phase * juce::MathConstants<double>::twoPi); break;  // Sine
            case 1: out = (phase < 0.5) ? (phase * 4.0 - 1.0) : (3.0 - phase * 4.0); break; // Triangle
            case 2: out = 1.0 - 2.0 * phase; break;       // Saw
            case 3: out = 2.0 * phase - 1.0; break;       // RevSaw
            case 4: out = (phase < 0.5) ? 1.0 : -1.0; break; // Square
            case 5: // S&H — resample on every phase reset
            {
                // Capture old phase before advance, detect wrap
                double oldPhase = phase;
                double inc = freqHz / sampleRate;
                double newPhase = oldPhase + inc;
                if (newPhase >= 1.0 || oldPhase == 0.0)  // wrapped or freshly reset
                    holdValue = (double)juce::Random::getSystemRandom().nextFloat() * 2.0 - 1.0;
                out = holdValue;
                // Advance phase manually here; skip the common advance below
                phase = newPhase >= 1.0 ? newPhase - 1.0 : newPhase;
                return out;
            }
            default: break;
        }
        phase += freqHz / sampleRate;
        if (phase >= 1.0) phase -= 1.0;
        return out;
    }
};

//==============================================================================
// Voice
struct WaspVoice
{
    int    midiNote  = -1;
    double baseFreq  = 0.0;
    bool   noteHeld  = false;  // true while MIDI key is physically depressed
    bool   isActive  = false;  // true while amp envelope is producing sound (including release)
    float  velocity  = 1.0f;   // normalised 0..1 note-on velocity

    // Oscillator phases (main pair)
    double osc1Phase = 0.0;
    double osc2Phase = 0.0;
    double osc3Phase = 0.0;

    // Detuned "dual voice" companion phases (second unison layer, XT "DUAL" switch)
    double osc1PhaseB = 0.0;
    double osc2PhaseB = 0.0;

    WaspFilter filter;
    WaspEnv    ampEnv;
    WaspEnv    fltEnv;
    WaspModEnv modEnv;

    double sampleRate = 44100.0;

    void setSampleRate (double sr)
    {
        sampleRate = sr;
        filter.setSampleRate (sr);
        ampEnv.setSampleRate (sr);
        fltEnv.setSampleRate (sr);
        modEnv.setSampleRate (sr);
    }

    void noteOn (int note, double freq, float vel)
    {
        midiNote   = note;
        baseFreq   = freq;
        noteHeld   = true;
        isActive   = true;
        velocity   = vel;
        osc1Phase = osc2Phase = osc3Phase = 0.0;
        osc1PhaseB = osc2PhaseB = 0.0;
        ampEnv.noteOn();
        fltEnv.noteOn();
        modEnv.noteOn();
    }

    void noteOff()
    {
        noteHeld = false;
        ampEnv.noteOff();
        fltEnv.noteOff();
    }

    // Generate one sample of a given oscillator waveform
    float oscSample (double& phase, double freq, int shape, double pw)
    {
        float out = 0.0f;
        switch (shape)
        {
            case 0: // Saw (with trivial PolyBLEP not implemented — simple for now)
                out = (float)(1.0 - 2.0 * phase);
                break;
            case 1: // Square (with PW)
            {
                double pwClamped = juce::jlimit (0.05, 0.95, pw);
                out = (phase < pwClamped) ? 1.0f : -1.0f;
                break;
            }
            case 2: // Triangle
                out = (float)((phase < 0.5) ? (4.0 * phase - 1.0) : (3.0 - 4.0 * phase));
                break;
            case 3: // Sine
                out = (float)std::sin (phase * juce::MathConstants<double>::twoPi);
                break;
            default: break;
        }
        phase += freq / sampleRate;
        if (phase >= 1.0) phase -= 1.0;
        return out;
    }

    // Generate one sample for the sub/noise oscillator
    float osc3Sample (double& phase, double baseFreq_, int shape)
    {
        float out = 0.0f;
        switch (shape)
        {
            case 0: // Sub (-1 octave square)
            {
                double f = baseFreq_ * 0.5;
                out = (phase < 0.5) ? 1.0f : -1.0f;
                phase += f / sampleRate;
                if (phase >= 1.0) phase -= 1.0;
                break;
            }
            case 1: // Sub-sub (-2 octave square)
            {
                double f = baseFreq_ * 0.25;
                out = (phase < 0.5) ? 1.0f : -1.0f;
                phase += f / sampleRate;
                if (phase >= 1.0) phase -= 1.0;
                break;
            }
            case 2: // Noise
                out = (float)(juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f);
                break;
            default: break;
        }
        return out;
    }
};

//==============================================================================
// Main Processor
//==============================================================================
class WASPAlphaAudioProcessor : public juce::AudioProcessor,
                                 public juce::AudioProcessorValueTreeState::Listener
{
public:
    WASPAlphaAudioProcessor();
    ~WASPAlphaAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "WASPAlpha"; }
    bool acceptsMidi()  const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 8.0; }  // matches max release param

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void parameterChanged (const juce::String& paramID, float newValue) override;

    juce::AudioProcessorValueTreeState apvts;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    // Voices (max 8 + dual voice doubles it conceptually)
    static constexpr int MAX_VOICES = 8;
    WaspVoice voices[MAX_VOICES];

    // LFOs (shared across voices)
    WaspLfo lfo1, lfo2;

    // Distortion HP filter (tone shaping)
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                   juce::dsp::IIR::Coefficients<float>> distToneFilter;

    double currentSampleRate = 44100.0;
    int    currentBlockSize  = 512;

    // MIDI handling
    void handleMidiEvent (const juce::MidiMessage& msg);
    int  findFreeVoice();
    int  findVoiceForNote (int midiNote);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WASPAlphaAudioProcessor)
};
