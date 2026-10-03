#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WASPAlphaAudioProcessorEditor::WASPAlphaAudioProcessorEditor (WASPAlphaAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    using namespace WaspParams;

    setSize (kWindowW, kWindowH);
    setResizable (false, false);

    // ===== OSC 1 =====
    setupCombo (osc1Shape,  { "SAW", "SQR", "TRI", "SIN" });
    addAndMakeVisible (secOsc);
    addAndMakeVisible (osc1Shape);
    addAndMakeVisible (osc1Coarse);
    addAndMakeVisible (osc1Fine);
    addAndMakeVisible (osc2Shape);
    addAndMakeVisible (osc2Coarse);
    addAndMakeVisible (osc2Fine);
    addAndMakeVisible (osc3Shape);
    addAndMakeVisible (osc3Amount);

    setupCombo (osc2Shape,  { "SAW", "SQR", "TRI", "SIN" });
    setupCombo (osc3Shape,  { "SUB", "SUB2", "NOISE" });

    // ===== MIX / MOD =====
    addAndMakeVisible (secMix);
    addAndMakeVisible (mixKnob);
    addAndMakeVisible (pwKnob);
    addAndMakeVisible (fmKnob);
    addAndMakeVisible (rmKnob);

    // ===== FILTER =====
    setupCombo (fltType, { "LP12", "LP24", "HP12", "BP12" });
    addAndMakeVisible (secFilter);
    addAndMakeVisible (fltType);
    addAndMakeVisible (cutoff);
    addAndMakeVisible (resonance);
    addAndMakeVisible (fltEnvAmt);
    addAndMakeVisible (fltKT);

    // ===== ENVELOPES =====
    addAndMakeVisible (secAmpEnv);
    addAndMakeVisible (ampA);
    addAndMakeVisible (ampD);
    addAndMakeVisible (ampS);
    addAndMakeVisible (ampR);

    addAndMakeVisible (secFltEnv);
    addAndMakeVisible (fltA);
    addAndMakeVisible (fltD);
    addAndMakeVisible (fltS);
    addAndMakeVisible (fltR);

    // ===== LFO 1 =====
    setupCombo (lfo1Shape,  { "SIN", "TRI", "SAW", "RSAW", "SQR", "S+H" });
    setupCombo (lfo1Target, { "PITCH", "OSC1", "OSC2", "PW", "FILTER", "AMP" });
    lfo1Sync.setLookAndFeel (&laf);
    lfo1Reset.setLookAndFeel (&laf);
    addAndMakeVisible (secLfo1);
    addAndMakeVisible (lfo1Shape);
    addAndMakeVisible (lfo1Target);
    addAndMakeVisible (lfo1Freq);
    addAndMakeVisible (lfo1Amount);
    addAndMakeVisible (lfo1Delay);
    addAndMakeVisible (lfo1Sync);
    addAndMakeVisible (lfo1Reset);

    // ===== LFO 2 =====
    setupCombo (lfo2Shape,  { "SIN", "TRI", "SAW", "RSAW", "SQR", "S+H" });
    setupCombo (lfo2Target, { "PITCH", "OSC1", "OSC2", "PW", "FILTER", "AMP" });
    lfo2Sync.setLookAndFeel (&laf);
    lfo2Reset.setLookAndFeel (&laf);
    addAndMakeVisible (secLfo2);
    addAndMakeVisible (lfo2Shape);
    addAndMakeVisible (lfo2Target);
    addAndMakeVisible (lfo2Freq);
    addAndMakeVisible (lfo2Amount);
    addAndMakeVisible (lfo2Delay);
    addAndMakeVisible (lfo2Sync);
    addAndMakeVisible (lfo2Reset);

    // ===== DISTORTION =====
    distOn.setLookAndFeel (&laf);
    addAndMakeVisible (secFx);
    addAndMakeVisible (distOn);
    addAndMakeVisible (distDrive);
    addAndMakeVisible (distTone);

    // ===== CHARACTER =====
    cmosFilter.setLookAndFeel (&laf);
    addAndMakeVisible (secChar);
    addAndMakeVisible (dualVoice);
    addAndMakeVisible (waspChar);
    addAndMakeVisible (vcoBinary);
    addAndMakeVisible (cmosFilter);

    // ===== CV =====
    setupCombo (cvTarget, { "FILTER", "OSC2 PITCH", "PW", "AMP" });
    addAndMakeVisible (secCV);
    addAndMakeVisible (cvAmount);
    addAndMakeVisible (cvTarget);

    // ===== MOD ENV =====
    setupCombo (modEnvDest, { "PW", "LFO1 AMT", "OSC1 LVL", "OSC2 PITCH" });
    addAndMakeVisible (secModEnv);
    addAndMakeVisible (modEnvAtk);
    addAndMakeVisible (modEnvDec);
    addAndMakeVisible (modEnvAmt);
    addAndMakeVisible (modEnvDest);

    // ===== VELOCITY =====
    addAndMakeVisible (secVelocity);
    addAndMakeVisible (velToAmp);
    addAndMakeVisible (velToFlt);

    // ===== UNISON (Dual Voice Detune) =====
    addAndMakeVisible (secUnison);
    addAndMakeVisible (dualDetune);

    // ===== FILTER 2 =====
    setupCombo (flt2Type, { "LP12", "LP24", "HP12", "BP12" });
    addAndMakeVisible (secFilter2);
    addAndMakeVisible (flt2Type);
    addAndMakeVisible (cutoff2);
    addAndMakeVisible (resonance2);
    addAndMakeVisible (flt2EnvAmt);
    addAndMakeVisible (flt2KT);

    // ===== FILTER ROUTING =====
    setupCombo (fltRouting, { "FILTER 1 ONLY", "FILTER 2 ONLY", "SERIES 1->2", "PARALLEL" });
    addAndMakeVisible (secRouting);
    addAndMakeVisible (fltRouting);
    addAndMakeVisible (fltParallelMix);

    // ===== DELAY =====
    addAndMakeVisible (secDelay);
    addAndMakeVisible (delayTime);
    addAndMakeVisible (delayFeedback);
    addAndMakeVisible (delayMix);

    // ===== OUTPUT =====
    addAndMakeVisible (secOutput);
    addAndMakeVisible (masterVolume);

    // ===== APVTS ATTACHMENTS =====
    attOsc1Shape  = std::make_unique<ComboAtt>  (audioProcessor.apvts, OSC1_SHAPE,  osc1Shape);
    attOsc1Coarse = std::make_unique<SliderAtt> (audioProcessor.apvts, OSC1_COARSE, osc1Coarse.slider);
    attOsc1Fine   = std::make_unique<SliderAtt> (audioProcessor.apvts, OSC1_FINE,   osc1Fine.slider);
    attOsc2Shape  = std::make_unique<ComboAtt>  (audioProcessor.apvts, OSC2_SHAPE,  osc2Shape);
    attOsc2Coarse = std::make_unique<SliderAtt> (audioProcessor.apvts, OSC2_COARSE, osc2Coarse.slider);
    attOsc2Fine   = std::make_unique<SliderAtt> (audioProcessor.apvts, OSC2_FINE,   osc2Fine.slider);
    attOsc3Shape  = std::make_unique<ComboAtt>  (audioProcessor.apvts, OSC3_SHAPE,  osc3Shape);
    attOsc3Amount = std::make_unique<SliderAtt> (audioProcessor.apvts, OSC3_AMOUNT, osc3Amount.slider);

    attMix = std::make_unique<SliderAtt> (audioProcessor.apvts, OSC_MIX,     mixKnob.slider);
    attPW  = std::make_unique<SliderAtt> (audioProcessor.apvts, PULSE_WIDTH, pwKnob.slider);
    attFM  = std::make_unique<SliderAtt> (audioProcessor.apvts, FM_AMOUNT,   fmKnob.slider);
    attRM  = std::make_unique<SliderAtt> (audioProcessor.apvts, RM_AMOUNT,   rmKnob.slider);

    attFltType   = std::make_unique<ComboAtt>  (audioProcessor.apvts, FLT_TYPE,    fltType);
    attCutoff    = std::make_unique<SliderAtt> (audioProcessor.apvts, FLT_CUTOFF,  cutoff.slider);
    attReso      = std::make_unique<SliderAtt> (audioProcessor.apvts, FLT_RESO,    resonance.slider);
    attFltEnvAmt = std::make_unique<SliderAtt> (audioProcessor.apvts, FLT_ENV_AMT, fltEnvAmt.slider);
    attFltKT     = std::make_unique<SliderAtt> (audioProcessor.apvts, FLT_KEYTRACK, fltKT.slider);

    attAmpA = std::make_unique<SliderAtt> (audioProcessor.apvts, AMP_ATTACK,  ampA.slider);
    attAmpD = std::make_unique<SliderAtt> (audioProcessor.apvts, AMP_DECAY,   ampD.slider);
    attAmpS = std::make_unique<SliderAtt> (audioProcessor.apvts, AMP_SUSTAIN, ampS.slider);
    attAmpR = std::make_unique<SliderAtt> (audioProcessor.apvts, AMP_RELEASE, ampR.slider);

    attFltA = std::make_unique<SliderAtt> (audioProcessor.apvts, FLT_ATTACK,  fltA.slider);
    attFltD = std::make_unique<SliderAtt> (audioProcessor.apvts, FLT_DECAY,   fltD.slider);
    attFltS = std::make_unique<SliderAtt> (audioProcessor.apvts, FLT_SUSTAIN, fltS.slider);
    attFltR = std::make_unique<SliderAtt> (audioProcessor.apvts, FLT_RELEASE, fltR.slider);

    attLfo1Shape = std::make_unique<ComboAtt>  (audioProcessor.apvts, LFO1_SHAPE,  lfo1Shape);
    attLfo1Target= std::make_unique<ComboAtt>  (audioProcessor.apvts, LFO1_TARGET, lfo1Target);
    attLfo1Freq  = std::make_unique<SliderAtt> (audioProcessor.apvts, LFO1_FREQ,   lfo1Freq.slider);
    attLfo1Amt   = std::make_unique<SliderAtt> (audioProcessor.apvts, LFO1_AMOUNT, lfo1Amount.slider);
    attLfo1Delay = std::make_unique<SliderAtt> (audioProcessor.apvts, LFO1_DELAY,  lfo1Delay.slider);
    attLfo1Sync  = std::make_unique<ButtonAtt> (audioProcessor.apvts, LFO1_SYNC,   lfo1Sync);
    attLfo1Reset = std::make_unique<ButtonAtt> (audioProcessor.apvts, LFO1_RESET,  lfo1Reset);

    attLfo2Shape = std::make_unique<ComboAtt>  (audioProcessor.apvts, LFO2_SHAPE,  lfo2Shape);
    attLfo2Target= std::make_unique<ComboAtt>  (audioProcessor.apvts, LFO2_TARGET, lfo2Target);
    attLfo2Freq  = std::make_unique<SliderAtt> (audioProcessor.apvts, LFO2_FREQ,   lfo2Freq.slider);
    attLfo2Amt   = std::make_unique<SliderAtt> (audioProcessor.apvts, LFO2_AMOUNT, lfo2Amount.slider);
    attLfo2Delay = std::make_unique<SliderAtt> (audioProcessor.apvts, LFO2_DELAY,  lfo2Delay.slider);
    attLfo2Sync  = std::make_unique<ButtonAtt> (audioProcessor.apvts, LFO2_SYNC,   lfo2Sync);
    attLfo2Reset = std::make_unique<ButtonAtt> (audioProcessor.apvts, LFO2_RESET,  lfo2Reset);

    attDistOn    = std::make_unique<ButtonAtt> (audioProcessor.apvts, DIST_ON,     distOn);
    attDistDrive = std::make_unique<SliderAtt> (audioProcessor.apvts, DIST_DRIVE,  distDrive.slider);
    attDistTone  = std::make_unique<SliderAtt> (audioProcessor.apvts, DIST_TONE,   distTone.slider);

    attDualVoice  = std::make_unique<SliderAtt> (audioProcessor.apvts, DUAL_VOICE,  dualVoice.slider);
    attDualDetune = std::make_unique<SliderAtt> (audioProcessor.apvts, DUAL_DETUNE, dualDetune.slider);
    attWaspChar   = std::make_unique<SliderAtt> (audioProcessor.apvts, WASP_CHAR,   waspChar.slider);
    attVCOBinary  = std::make_unique<SliderAtt> (audioProcessor.apvts, VCO_BINARY,  vcoBinary.slider);
    attCmosFilter = std::make_unique<ButtonAtt> (audioProcessor.apvts, CMOS_FILTER, cmosFilter);
    attCVAmount   = std::make_unique<SliderAtt> (audioProcessor.apvts, CV_AMOUNT,   cvAmount.slider);
    attCVTarget   = std::make_unique<ComboAtt>  (audioProcessor.apvts, CV_TARGET,   cvTarget);

    attModEnvAtk  = std::make_unique<SliderAtt> (audioProcessor.apvts, MODENV_ATTACK, modEnvAtk.slider);
    attModEnvDec  = std::make_unique<SliderAtt> (audioProcessor.apvts, MODENV_DECAY,  modEnvDec.slider);
    attModEnvAmt  = std::make_unique<SliderAtt> (audioProcessor.apvts, MODENV_AMOUNT, modEnvAmt.slider);
    attModEnvDest = std::make_unique<ComboAtt>  (audioProcessor.apvts, MODENV_DEST,   modEnvDest);

    attVelToAmp = std::make_unique<SliderAtt> (audioProcessor.apvts, VEL_TO_AMP,    velToAmp.slider);
    attVelToFlt = std::make_unique<SliderAtt> (audioProcessor.apvts, VEL_TO_FILTER, velToFlt.slider);

    attFlt2Type   = std::make_unique<ComboAtt>  (audioProcessor.apvts, FLT2_TYPE,      flt2Type);
    attCutoff2    = std::make_unique<SliderAtt> (audioProcessor.apvts, FLT2_CUTOFF,    cutoff2.slider);
    attReso2      = std::make_unique<SliderAtt> (audioProcessor.apvts, FLT2_RESO,      resonance2.slider);
    attFlt2EnvAmt = std::make_unique<SliderAtt> (audioProcessor.apvts, FLT2_ENV_AMT,   flt2EnvAmt.slider);
    attFlt2KT     = std::make_unique<SliderAtt> (audioProcessor.apvts, FLT2_KEYTRACK,  flt2KT.slider);
    attFltRouting = std::make_unique<ComboAtt>  (audioProcessor.apvts, FLT_ROUTING,    fltRouting);
    attFltParallelMix = std::make_unique<SliderAtt> (audioProcessor.apvts, FLT_PARALLEL_MIX, fltParallelMix.slider);

    attDelayTime     = std::make_unique<SliderAtt> (audioProcessor.apvts, DELAY_TIME_MS,  delayTime.slider);
    attDelayFeedback = std::make_unique<SliderAtt> (audioProcessor.apvts, DELAY_FEEDBACK, delayFeedback.slider);
    attDelayMix      = std::make_unique<SliderAtt> (audioProcessor.apvts, DELAY_MIX,      delayMix.slider);

    attMasterVolume  = std::make_unique<SliderAtt> (audioProcessor.apvts, MASTER_VOLUME,  masterVolume.slider);
}

WASPAlphaAudioProcessorEditor::~WASPAlphaAudioProcessorEditor()
{
    osc1Shape.setLookAndFeel (nullptr);
    osc2Shape.setLookAndFeel (nullptr);
    osc3Shape.setLookAndFeel (nullptr);
    fltType.setLookAndFeel (nullptr);
    lfo1Shape.setLookAndFeel (nullptr);
    lfo1Target.setLookAndFeel (nullptr);
    lfo1Sync.setLookAndFeel (nullptr);
    lfo1Reset.setLookAndFeel (nullptr);
    lfo2Shape.setLookAndFeel (nullptr);
    lfo2Target.setLookAndFeel (nullptr);
    lfo2Sync.setLookAndFeel (nullptr);
    lfo2Reset.setLookAndFeel (nullptr);
    distOn.setLookAndFeel (nullptr);
    cmosFilter.setLookAndFeel (nullptr);
    cvTarget.setLookAndFeel (nullptr);
    modEnvDest.setLookAndFeel (nullptr);
    flt2Type.setLookAndFeel (nullptr);
    fltRouting.setLookAndFeel (nullptr);
}

//==============================================================================
void WASPAlphaAudioProcessorEditor::setupCombo (juce::ComboBox& box,
                                                  const juce::StringArray& items)
{
    box.setLookAndFeel (&laf);
    int id = 1;
    for (auto& item : items)
        box.addItem (item, id++);
    box.setSelectedId (1, juce::dontSendNotification);
}

//==============================================================================
void WASPAlphaAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Main background
    g.fillAll (juce::Colour (WaspLookAndFeel::COL_BODY_BG));

    // Subtle horizontal stripe texture (WASP had that ribbed panel look)
    g.setColour (juce::Colour (WaspLookAndFeel::COL_WASP_STRIPE).withAlpha (0.4f));
    for (int y = 0; y < getHeight(); y += 4)
        g.fillRect (0, y, getWidth(), 1);

    // Header bar — near-black base with an orange->red gradient blur across it
    juce::Rectangle<int> header (0, 0, getWidth(), 34);
    g.setColour (juce::Colour (0xff0c0605u));
    g.fillRect (header);
    juce::ColourGradient hdrGrad (juce::Colour (WaspLookAndFeel::COL_GRAD_ORANGE).withAlpha (0.20f), 0.0f, 0.0f,
                                   juce::Colour (WaspLookAndFeel::COL_GRAD_RED).withAlpha (0.28f), (float)getWidth(), 0.0f, false);
    g.setGradientFill (hdrGrad);
    g.fillRect (header);

    // Header bottom accent — full gradient bar
    juce::ColourGradient accentGrad (juce::Colour (WaspLookAndFeel::COL_GRAD_ORANGE), 0.0f, 33.0f,
                                      juce::Colour (WaspLookAndFeel::COL_GRAD_RED), (float)getWidth(), 33.0f, false);
    g.setGradientFill (accentGrad);
    g.fillRect (0, 33, getWidth(), 2);

    // Plugin name
    g.setColour (juce::Colour (WaspLookAndFeel::COL_WASP_YELLOW));
    g.setFont (juce::FontOptions ("Courier New", 18.0f, juce::Font::bold));
    g.drawText ("KillerBee", 10, 0, 260, 34, juce::Justification::centredLeft, false);

    // Subtitle
    g.setColour (juce::Colour (WaspLookAndFeel::COL_TEXT_SECONDARY));
    g.setFont (juce::FontOptions ("Courier New", 9.0f, juce::Font::plain));
    g.drawText ("AlphaAudio  |  WASP/XT Tribute + Virtual Analog Hybrid  |  William Ashley Music",
                260, 0, getWidth() - 270, 34, juce::Justification::centredRight, false);

    // Orange/red dot accents in header
    g.setColour (juce::Colour (WaspLookAndFeel::COL_WASP_AMBER));
    for (int i = 0; i < 5; ++i)
        g.fillEllipse ((float)(getWidth() - 30 + i * 0), 13.0f, 6.0f, 6.0f);

    // Sub-osc label
    g.setColour (juce::Colour (WaspLookAndFeel::COL_TEXT_SECONDARY));
    g.setFont (juce::FontOptions ("Courier New", 8.5f, juce::Font::plain));
    g.drawText ("OSC 1", 14, 40, 120, 12, juce::Justification::centredLeft, false);
    g.drawText ("OSC 2", 232, 40, 120, 12, juce::Justification::centredLeft, false);
    g.drawText ("OSC 3/SUB", 440, 40, 120, 12, juce::Justification::centredLeft, false);
}

//==============================================================================
void WASPAlphaAudioProcessorEditor::resized()
{
    // Layout constants
    constexpr int PAD  = 5;
    constexpr int TOP  = 36;
    constexpr int HDR  = 16; // section header height included in section bounds
    constexpr int GAP  = 8;  // standard breathing room between adjacent controls
                              // (was 4 in a few spots — that's what was reading as
                              // "dropdowns going over the knobs"; widened throughout)

    int w = getWidth();
    int h = getHeight();

    // =====================================================================
    // ROW 1: Oscillators (3 osc blocks) | Mix/Mod | Character
    // =====================================================================
    int row1Y = TOP + 6;
    int row1H = 110;
    int kSize = 54;
    int comboH = 18;

    // Osc 1 — shape combo gets its own clear strip; knobs start well clear of it
    int oc1x = 10;
    osc1Shape.setBounds (oc1x, row1Y + HDR + 2, 70, comboH);
    osc1Coarse.setBounds (oc1x + 70 + GAP, row1Y + HDR + 2, kSize, kSize + 14);
    osc1Fine.setBounds   (oc1x + 70 + GAP + kSize + GAP, row1Y + HDR + 2, kSize, kSize + 14);
    int osc1End = oc1x + 70 + GAP + kSize + GAP + kSize; // = 10+70+8+54+8+54 = 204

    // Osc 2 — starts safely after osc1 ends (this is the exact spot that used to overlap)
    int o2x = osc1End + GAP + 6;
    osc2Shape.setBounds (o2x, row1Y + HDR + 2, 70, comboH);
    osc2Coarse.setBounds (o2x + 70 + GAP, row1Y + HDR + 2, kSize, kSize + 14);
    osc2Fine.setBounds   (o2x + 70 + GAP + kSize + GAP, row1Y + HDR + 2, kSize, kSize + 14);
    int osc2End = o2x + 70 + GAP + kSize + GAP + kSize;

    // Osc 3
    int o3x = osc2End + GAP + 6;
    osc3Shape.setBounds (o3x, row1Y + HDR + 2, 60, comboH);
    osc3Amount.setBounds (o3x + 60 + GAP, row1Y + HDR + 2, kSize, kSize + 14);
    int osc3End = o3x + 60 + GAP + kSize;

    secOsc.setBounds (PAD, row1Y, (osc3End - PAD) + 6, row1H); // spans exactly what the 3 oscs use

    // Mix/Mod section — starts right after the oscillators, not at a hardcoded offset
    int mixX = osc3End + 6 + PAD;
    secMix.setBounds (mixX, row1Y, 4 * (kSize + GAP) + 8, row1H);
    mixKnob.setBounds (mixX + 6,                     row1Y + HDR + 2, kSize, kSize + 14);
    pwKnob.setBounds  (mixX + 6 + (kSize + GAP),     row1Y + HDR + 2, kSize, kSize + 14);
    fmKnob.setBounds  (mixX + 6 + 2*(kSize + GAP),   row1Y + HDR + 2, kSize, kSize + 14);
    rmKnob.setBounds  (mixX + 6 + 3*(kSize + GAP),   row1Y + HDR + 2, kSize, kSize + 14);
    // CHARACTER previously lived here too, cramming the row past the window edge —
    // it now lives in Row 4 where there's actually room for it (see below).

    // =====================================================================
    // ROW 2: Filter 1 | AmpEnv | FltEnv
    // =====================================================================
    int row2Y = row1Y + row1H + PAD;
    int row2H = 118;

    // Filter 1 section
    int fltW = 4 * (kSize + GAP) + 78;
    secFilter.setBounds (PAD, row2Y, fltW, row2H);
    fltType.setBounds      (PAD + 6,                    row2Y + HDR + 2, 70, comboH);
    cutoff.setBounds       (PAD + 6 + 70 + GAP,             row2Y + HDR + 2, kSize, kSize + 14);
    resonance.setBounds    (PAD + 6 + 70 + GAP + (kSize+GAP),   row2Y + HDR + 2, kSize, kSize + 14);
    fltEnvAmt.setBounds    (PAD + 6 + 70 + GAP + 2*(kSize+GAP), row2Y + HDR + 2, kSize, kSize + 14);
    fltKT.setBounds        (PAD + 6 + 70 + GAP + 3*(kSize+GAP), row2Y + HDR + 2, kSize, kSize + 14);

    // Amp Envelope
    int ampX = PAD + fltW + PAD;
    int envW = 4 * (kSize + GAP) + 8;
    secAmpEnv.setBounds (ampX, row2Y, envW, row2H);
    ampA.setBounds (ampX + 6,                 row2Y + HDR + 2, kSize, kSize + 14);
    ampD.setBounds (ampX + 6 + (kSize+GAP),   row2Y + HDR + 2, kSize, kSize + 14);
    ampS.setBounds (ampX + 6 + 2*(kSize+GAP), row2Y + HDR + 2, kSize, kSize + 14);
    ampR.setBounds (ampX + 6 + 3*(kSize+GAP), row2Y + HDR + 2, kSize, kSize + 14);

    // Filter Envelope
    int fenvX = ampX + envW + PAD;
    secFltEnv.setBounds (fenvX, row2Y, envW, row2H);
    fltA.setBounds (fenvX + 6,                 row2Y + HDR + 2, kSize, kSize + 14);
    fltD.setBounds (fenvX + 6 + (kSize+GAP),   row2Y + HDR + 2, kSize, kSize + 14);
    fltS.setBounds (fenvX + 6 + 2*(kSize+GAP), row2Y + HDR + 2, kSize, kSize + 14);
    fltR.setBounds (fenvX + 6 + 3*(kSize+GAP), row2Y + HDR + 2, kSize, kSize + 14);

    // =====================================================================
    // ROW 3: LFO1 | LFO2 | Distortion | CV
    // =====================================================================
    int row3Y = row2Y + row2H + PAD;
    int row3H = 118;

    // LFO 1 — shape/target combos on their own strip, knobs start clearly below them
    int lfoW = 270;
    secLfo1.setBounds (PAD, row3Y, lfoW, row3H);

    int lComboW = 74;
    int lfoKnobY = row3Y + HDR + 2 + comboH + GAP; // combos end, then a full GAP before knobs
    lfo1Shape.setBounds  (PAD + 6,                  row3Y + HDR + 2, lComboW, comboH);
    lfo1Target.setBounds (PAD + 6 + lComboW + GAP,  row3Y + HDR + 2, lComboW, comboH);
    lfo1Freq.setBounds   (PAD + 6,                  lfoKnobY, kSize, kSize + 14);
    lfo1Amount.setBounds (PAD + 6 + (kSize+GAP),    lfoKnobY, kSize, kSize + 14);
    lfo1Delay.setBounds  (PAD + 6 + 2*(kSize+GAP),  lfoKnobY, kSize, kSize + 14);
    lfo1Sync.setBounds   (PAD + 6 + 3*(kSize+GAP),  row3Y + HDR + 2,  66, 18);
    lfo1Reset.setBounds  (PAD + 6 + 3*(kSize+GAP),  row3Y + HDR + 24, 66, 18);

    // LFO 2
    int lfo2X = PAD + lfoW + PAD;
    secLfo2.setBounds (lfo2X, row3Y, lfoW, row3H);

    lfo2Shape.setBounds  (lfo2X + 6,                  row3Y + HDR + 2, lComboW, comboH);
    lfo2Target.setBounds (lfo2X + 6 + lComboW + GAP,  row3Y + HDR + 2, lComboW, comboH);
    lfo2Freq.setBounds   (lfo2X + 6,                  lfoKnobY, kSize, kSize + 14);
    lfo2Amount.setBounds (lfo2X + 6 + (kSize+GAP),    lfoKnobY, kSize, kSize + 14);
    lfo2Delay.setBounds  (lfo2X + 6 + 2*(kSize+GAP),  lfoKnobY, kSize, kSize + 14);
    lfo2Sync.setBounds   (lfo2X + 6 + 3*(kSize+GAP),  row3Y + HDR + 2,  66, 18);
    lfo2Reset.setBounds  (lfo2X + 6 + 3*(kSize+GAP),  row3Y + HDR + 24, 66, 18);

    // Distortion
    int distX = lfo2X + lfoW + PAD;
    int distW = 2 * (kSize + GAP) + 28;
    secFx.setBounds (distX, row3Y, distW, row3H);
    distOn.setBounds    (distX + 8,               row3Y + HDR + 6,  60, 18);
    distDrive.setBounds (distX + 8,               row3Y + HDR + 30, kSize, kSize + 14);
    distTone.setBounds  (distX + 8 + kSize + GAP, row3Y + HDR + 30, kSize, kSize + 14);

    // CV (Control Voltage) — amount knob left, target combo stacked below it, not beside it,
    // so the combo never sits at the same height as (and can't visually clash with) the knob
    int cvX = distX + distW + PAD;
    int cvW = w - cvX - PAD;
    secCV.setBounds (cvX, row3Y, cvW, row3H);
    cvAmount.setBounds (cvX + 6, row3Y + HDR + 2, kSize, kSize + 14);
    cvTarget.setBounds (cvX + 6, row3Y + HDR + 2 + kSize + 14 + GAP, juce::jmax (90, cvW - 16), comboH);

    // =====================================================================
    // ROW 4: Mod Env | Velocity | Unison Detune
    // =====================================================================
    int row4Y = row3Y + row3H + PAD;
    int row4H = 118; // matches row2/row3 height so the combo strip under Mod Env fits cleanly

    // Mod Env — ATK/DEC/AMT knobs on top, destination combo on its own strip below
    // (previously the combo sat beside the knobs at knob height — moved below instead)
    int modEnvW = 3 * (kSize + GAP) + 8;
    secModEnv.setBounds (PAD, row4Y, modEnvW, row4H);
    modEnvAtk.setBounds  (PAD + 6,                 row4Y + HDR + 2, kSize, kSize + 14);
    modEnvDec.setBounds  (PAD + 6 + (kSize+GAP),   row4Y + HDR + 2, kSize, kSize + 14);
    modEnvAmt.setBounds  (PAD + 6 + 2*(kSize+GAP), row4Y + HDR + 2, kSize, kSize + 14);

    // Mod Env destination combo — full width strip beneath the knob row
    int modEnvDestY = row4Y + HDR + 2 + kSize + 14 + GAP;
    modEnvDest.setBounds (PAD + 6, modEnvDestY, modEnvW - 12, comboH);

    // Velocity — depth of note velocity into Amp Env and Filter Env
    int velX = PAD + modEnvW + PAD;
    int velW = 2 * (kSize + GAP) + 8;
    secVelocity.setBounds (velX, row4Y, velW, row4H);
    velToAmp.setBounds (velX + 6,               row4Y + HDR + 2, kSize, kSize + 14);
    velToFlt.setBounds (velX + 6 + (kSize+GAP), row4Y + HDR + 2, kSize, kSize + 14);

    // Unison (Dual Voice Detune)
    int detuneX = velX + velW + PAD;
    int detuneW = kSize + 20;
    secUnison.setBounds (detuneX, row4Y, detuneW, row4H);
    dualDetune.setBounds (detuneX + 8, row4Y + HDR + 2, kSize, kSize + 14);

    // Character — moved here from Row 1, which didn't have room for it.
    // Dual Voice | Wasp | VCO Binary knobs, CMOS toggle to the right of them.
    int charX = detuneX + detuneW + PAD;
    int charW = w - charX - PAD;
    secChar.setBounds (charX, row4Y, charW, row4H);
    dualVoice.setBounds  (charX + 6,                 row4Y + HDR + 2, kSize, kSize + 14);
    waspChar.setBounds   (charX + 6 + (kSize+GAP),   row4Y + HDR + 2, kSize, kSize + 14);
    vcoBinary.setBounds  (charX + 6 + 2*(kSize+GAP), row4Y + HDR + 2, kSize, kSize + 14);
    cmosFilter.setBounds (charX + 6 + 3*(kSize+GAP), row4Y + HDR + 2, 70, 20);

    // =====================================================================
    // ROW 5: Filter 2 | Filter Routing
    // =====================================================================
    int row5Y = row4Y + row4H + PAD;
    int row5H = h - row5Y - PAD;

    int flt2W = 4 * (kSize + GAP) + 78;
    secFilter2.setBounds (PAD, row5Y, flt2W, row5H);
    flt2Type.setBounds   (PAD + 6,                              row5Y + HDR + 2, 70, comboH);
    cutoff2.setBounds    (PAD + 6 + 70 + GAP,                       row5Y + HDR + 2, kSize, kSize + 14);
    resonance2.setBounds (PAD + 6 + 70 + GAP + (kSize+GAP),         row5Y + HDR + 2, kSize, kSize + 14);
    flt2EnvAmt.setBounds (PAD + 6 + 70 + GAP + 2*(kSize+GAP),       row5Y + HDR + 2, kSize, kSize + 14);
    flt2KT.setBounds     (PAD + 6 + 70 + GAP + 3*(kSize+GAP),       row5Y + HDR + 2, kSize, kSize + 14);

    // Filter Routing — combo on its own strip, mix knob below/beside with clear separation
    int routeX = PAD + flt2W + PAD;
    int routeW = 160 + GAP + kSize + 12; // fixed width now (was "rest of window" — needed room for Delay/Output)
    secRouting.setBounds (routeX, row5Y, routeW, row5H);
    fltRouting.setBounds     (routeX + 6, row5Y + HDR + 2, 160, comboH);
    fltParallelMix.setBounds (routeX + 6 + 160 + GAP, row5Y + HDR + 2, kSize, kSize + 14);

    // Delay — Time, Feedback, Mix
    int delayX = routeX + routeW + PAD;
    int delayW = 3 * (kSize + GAP) + 8;
    secDelay.setBounds (delayX, row5Y, delayW, row5H);
    delayTime.setBounds     (delayX + 6,                 row5Y + HDR + 2, kSize, kSize + 14);
    delayFeedback.setBounds (delayX + 6 + (kSize+GAP),   row5Y + HDR + 2, kSize, kSize + 14);
    delayMix.setBounds      (delayX + 6 + 2*(kSize+GAP), row5Y + HDR + 2, kSize, kSize + 14);

    // Output (Master Volume) — takes remaining width
    int outX = delayX + delayW + PAD;
    int outW = w - outX - PAD;
    secOutput.setBounds (outX, row5Y, outW, row5H);
    masterVolume.setBounds (outX + 6, row5Y + HDR + 2, kSize, kSize + 14);
}
