#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
// KillerBee Look and Feel
//==============================================================================
class WaspLookAndFeel : public juce::LookAndFeel_V4
{
public:
    // Palette
    static constexpr juce::uint32 COL_BODY_BG      = 0xff100807u;  // near-black, warm red undertone
    static constexpr juce::uint32 COL_PANEL_BG      = 0xff1a0d09u;  // panel sections
    static constexpr juce::uint32 COL_SECTION_HDR   = 0xff2a1206u;  // section header bars (base, gradient drawn over)
    static constexpr juce::uint32 COL_WASP_YELLOW   = 0xffff7a00u;  // primary accent: hot orange
    static constexpr juce::uint32 COL_WASP_AMBER    = 0xffe0281au;  // secondary accent: deep red
    static constexpr juce::uint32 COL_KNOB_BODY     = 0xff0a0a0au;  // knob body: black
    static constexpr juce::uint32 COL_KNOB_RING     = 0xff3a140au;  // knob ring: dark red-brown
    static constexpr juce::uint32 COL_KNOB_DOT      = 0xffff8800u;  // position indicator: orange
    static constexpr juce::uint32 COL_TEXT_PRIMARY   = 0xffff8800u;  // orange text
    static constexpr juce::uint32 COL_TEXT_SECONDARY = 0xffaa5020u;  // dimmer red-orange labels
    static constexpr juce::uint32 COL_OUTLINE        = 0xff4a1c10u;  // border outlines
    static constexpr juce::uint32 COL_BUTTON_OFF     = 0xff180808u;
    static constexpr juce::uint32 COL_BUTTON_ON      = 0xffff4400u;
    static constexpr juce::uint32 COL_WASP_STRIPE    = 0xff220f08u;  // subtle stripes
    static constexpr juce::uint32 COL_GRAD_ORANGE    = 0xffff8c00u;  // gradient blur endpoint 1
    static constexpr juce::uint32 COL_GRAD_RED       = 0xffcc1100u;  // gradient blur endpoint 2

    WaspLookAndFeel()
    {
        setColour (juce::Slider::thumbColourId,             juce::Colour (COL_WASP_YELLOW));
        setColour (juce::Slider::rotarySliderFillColourId,  juce::Colour (COL_WASP_AMBER));
        setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colour (COL_OUTLINE));
        setColour (juce::Slider::trackColourId,             juce::Colour (COL_OUTLINE));
        setColour (juce::Label::textColourId,               juce::Colour (COL_TEXT_PRIMARY));
        setColour (juce::ComboBox::backgroundColourId,      juce::Colour (COL_KNOB_BODY));
        setColour (juce::ComboBox::textColourId,            juce::Colour (COL_TEXT_PRIMARY));
        setColour (juce::ComboBox::outlineColourId,         juce::Colour (COL_OUTLINE));
        setColour (juce::ComboBox::arrowColourId,           juce::Colour (COL_WASP_YELLOW));
        setColour (juce::PopupMenu::backgroundColourId,     juce::Colour (COL_PANEL_BG));
        setColour (juce::PopupMenu::textColourId,           juce::Colour (COL_TEXT_PRIMARY));
        setColour (juce::PopupMenu::highlightedBackgroundColourId, juce::Colour (COL_WASP_YELLOW));
        setColour (juce::PopupMenu::highlightedTextColourId, juce::Colour (0xff000000u));
        setColour (juce::ToggleButton::textColourId,        juce::Colour (COL_TEXT_PRIMARY));
        setColour (juce::ToggleButton::tickColourId,        juce::Colour (COL_WASP_YELLOW));
        setColour (juce::ToggleButton::tickDisabledColourId, juce::Colour (COL_OUTLINE));
    }

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, juce::Slider& slider) override
    {
        auto bounds = juce::Rectangle<float> ((float)x, (float)y, (float)width, (float)height).reduced (4.0f);
        float radius    = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
        float centreX   = bounds.getCentreX();
        float centreY   = bounds.getCentreY();
        float angle     = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        // Shadow
        g.setColour (juce::Colour (0x66000000u));
        g.fillEllipse (centreX - radius + 2.0f, centreY - radius + 3.0f, radius * 2.0f, radius * 2.0f);

        // Knob body — black, with a faint radial highlight for depth (not olive anymore)
        juce::ColourGradient bodyGrad (juce::Colour (0xff2a2a2au), centreX - radius * 0.3f, centreY - radius * 0.3f,
                                        juce::Colour (0xff020202u), centreX + radius * 0.5f, centreY + radius * 0.5f,
                                        true);
        g.setGradientFill (bodyGrad);
        g.fillEllipse (centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f);

        // Ring arc (track) — dark red-brown base
        juce::Path trackArc;
        float trackRadius = radius + 3.0f;
        trackArc.addCentredArc (centreX, centreY, trackRadius, trackRadius, 0.0f,
                                 rotaryStartAngle, rotaryEndAngle, true);
        g.setColour (juce::Colour (COL_KNOB_RING));
        g.strokePath (trackArc, juce::PathStrokeType (2.0f));

        // Value arc — orange->red gradient blur, Virus-style
        juce::Path valueArc;
        valueArc.addCentredArc (centreX, centreY, trackRadius, trackRadius, 0.0f,
                                 rotaryStartAngle, angle, true);
        juce::ColourGradient arcGrad (juce::Colour (COL_GRAD_ORANGE), centreX + trackRadius * std::sin (rotaryStartAngle), centreY - trackRadius * std::cos (rotaryStartAngle),
                                       juce::Colour (COL_GRAD_RED), centreX + trackRadius * std::sin (rotaryEndAngle), centreY - trackRadius * std::cos (rotaryEndAngle),
                                       false);
        g.setGradientFill (arcGrad);
        g.strokePath (valueArc, juce::PathStrokeType (2.75f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Sharp black perimeter ring for definition
        g.setColour (juce::Colours::black);
        g.drawEllipse (centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f, 1.5f);

        // Thin bright highlight rim just inside the black ring (the "sharper" accent edge)
        g.setColour (juce::Colour (COL_WASP_YELLOW).withAlpha (0.55f));
        g.drawEllipse (centreX - radius + 1.5f, centreY - radius + 1.5f, (radius - 1.5f) * 2.0f, (radius - 1.5f) * 2.0f, 0.75f);

        // Indicator dot
        float dotDist = radius - 5.0f;
        float dotX    = centreX + dotDist * std::sin (angle);
        float dotY    = centreY - dotDist * std::cos (angle);
        g.setColour (juce::Colour (COL_KNOB_DOT));
        g.fillEllipse (dotX - 3.0f, dotY - 3.0f, 6.0f, 6.0f);

        // Centre dot
        g.setColour (juce::Colour (0xff1a1a1au));
        g.fillEllipse (centreX - 2.0f, centreY - 2.0f, 4.0f, 4.0f);
    }

    void drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                           bool shouldDrawButtonAsHighlighted,
                           bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds().toFloat().reduced (1.0f);
        bool toggled = button.getToggleState();

        // LED style toggle
        float ledSize = juce::jmin (bounds.getHeight() - 4.0f, 12.0f);
        float ledX    = bounds.getX() + 2.0f;
        float ledY    = bounds.getCentreY() - ledSize * 0.5f;

        g.setColour (toggled ? juce::Colour (COL_WASP_YELLOW) : juce::Colour (COL_BUTTON_OFF));
        g.fillEllipse (ledX, ledY, ledSize, ledSize);
        g.setColour (juce::Colours::black);
        g.drawEllipse (ledX, ledY, ledSize, ledSize, 1.0f);

        if (toggled)
        {
            // Glow — orange->red blur, not flat yellow
            juce::ColourGradient glowGrad (juce::Colour (COL_GRAD_ORANGE).withAlpha (0.45f), ledX + ledSize * 0.5f, ledY + ledSize * 0.5f,
                                            juce::Colour (COL_GRAD_RED).withAlpha (0.0f), ledX - 2.0f, ledY - 2.0f, true);
            g.setGradientFill (glowGrad);
            g.fillEllipse (ledX - 3.0f, ledY - 3.0f, ledSize + 6.0f, ledSize + 6.0f);
        }

        g.setColour (juce::Colour (toggled ? COL_TEXT_PRIMARY : COL_TEXT_SECONDARY));
        g.setFont (juce::FontOptions ("Courier New", 10.0f, juce::Font::bold));
        g.drawText (button.getButtonText(),
                    (int)(ledX + ledSize + 4.0f), (int)(bounds.getY()),
                    (int)(bounds.getWidth() - ledSize - 8.0f), (int)(bounds.getHeight()),
                    juce::Justification::centredLeft, false);
    }

    juce::Font getLabelFont (juce::Label& label) override
    {
        return juce::FontOptions ("Courier New", 10.0f, juce::Font::bold);
    }

    void drawComboBox (juce::Graphics& g, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       juce::ComboBox& box) override
    {
        auto bounds = juce::Rectangle<int> (0, 0, width, height).toFloat();
        g.setColour (juce::Colour (COL_KNOB_BODY)); // black body
        g.fillRoundedRectangle (bounds, 2.0f);
        // Thin orange->red gradient underline for a sharper, branded edge
        juce::ColourGradient underline (juce::Colour (COL_GRAD_ORANGE), bounds.getX(), bounds.getBottom(),
                                         juce::Colour (COL_GRAD_RED), bounds.getRight(), bounds.getBottom(), false);
        g.setGradientFill (underline);
        g.fillRect (bounds.getX(), bounds.getBottom() - 1.5f, bounds.getWidth(), 1.5f);
        g.setColour (juce::Colours::black);
        g.drawRoundedRectangle (bounds.reduced (0.5f), 2.0f, 1.0f);

        // Arrow
        juce::Path arrow;
        float ax = (float)(buttonX + buttonW / 2);
        float ay = (float)(height / 2 - 2);
        arrow.addTriangle (ax - 4.0f, ay, ax + 4.0f, ay, ax, ay + 5.0f);
        g.setColour (juce::Colour (COL_WASP_YELLOW));
        g.fillPath (arrow);
    }
};

//==============================================================================
// Small helper: labelled rotary knob
//==============================================================================
class WaspKnob : public juce::Component
{
public:
    juce::Slider slider;
    juce::Label  label;

    WaspKnob (const juce::String& labelText, WaspLookAndFeel& laf)
    {
        slider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
        slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
        slider.setLookAndFeel (&laf);
        addAndMakeVisible (slider);

        label.setText (labelText, juce::dontSendNotification);
        label.setJustificationType (juce::Justification::centred);
        label.setFont (juce::FontOptions ("Courier New", 9.0f, juce::Font::bold));
        label.setColour (juce::Label::textColourId, juce::Colour (WaspLookAndFeel::COL_TEXT_SECONDARY));
        label.setLookAndFeel (&laf);
        addAndMakeVisible (label);
    }

    ~WaspKnob() override
    {
        slider.setLookAndFeel (nullptr);
        label.setLookAndFeel (nullptr);
    }

    void resized() override
    {
        auto b = getLocalBounds();
        int labelH = 14;
        label.setBounds (b.removeFromBottom (labelH));
        slider.setBounds (b);
    }
};

//==============================================================================
// Section panel with labelled header
//==============================================================================
class WaspSection : public juce::Component
{
public:
    juce::String title;
    WaspSection (const juce::String& t) : title (t) {}

    void paint (juce::Graphics& g) override
    {
        auto b = getLocalBounds().toFloat();

        // Background — near-black panel
        g.setColour (juce::Colour (WaspLookAndFeel::COL_PANEL_BG));
        g.fillRoundedRectangle (b, 2.0f); // sharper corners than before (was 3.0f)

        // Sharp black perimeter border for definition
        g.setColour (juce::Colours::black);
        g.drawRoundedRectangle (b.reduced (0.5f), 2.0f, 1.2f);
        // Thin orange highlight just inside the border
        g.setColour (juce::Colour (WaspLookAndFeel::COL_WASP_YELLOW).withAlpha (0.25f));
        g.drawRoundedRectangle (b.reduced (2.0f), 1.5f, 0.6f);

        // Header bar — orange->red gradient blur (Virus-style)
        juce::Rectangle<float> header (b.getX(), b.getY(), b.getWidth(), 16.0f);
        juce::ColourGradient headerGrad (juce::Colour (WaspLookAndFeel::COL_GRAD_ORANGE).withAlpha (0.85f), header.getX(), header.getY(),
                                          juce::Colour (WaspLookAndFeel::COL_GRAD_RED).withAlpha (0.85f), header.getRight(), header.getY(), false);
        g.setGradientFill (headerGrad);
        g.fillRoundedRectangle (header, 2.0f);
        g.fillRect (header.withTrimmedTop (3.0f));

        // Title text — black on the bright gradient for contrast/sharpness
        g.setColour (juce::Colours::black);
        g.setFont (juce::FontOptions ("Courier New", 9.5f, juce::Font::bold));
        g.drawText (title, header.toNearestInt(), juce::Justification::centred, false);

        // Amber accent line under header
        g.setColour (juce::Colour (WaspLookAndFeel::COL_WASP_AMBER));
        g.fillRect (b.getX() + 2.0f, b.getY() + 15.0f, b.getWidth() - 4.0f, 1.0f);
    }
};

//==============================================================================
// Main Editor
//==============================================================================
class WASPAlphaAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    static constexpr int kWindowW = 900;
    static constexpr int kWindowH = 760;

    WASPAlphaAudioProcessorEditor (WASPAlphaAudioProcessor& p);
    ~WASPAlphaAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    WASPAlphaAudioProcessor& audioProcessor;
    WaspLookAndFeel laf;

    // --- Sections ---
    WaspSection secOsc      { "OSCILLATORS" };
    WaspSection secMix      { "MIX / MOD" };
    WaspSection secFilter   { "FILTER" };
    WaspSection secAmpEnv   { "AMP ENV" };
    WaspSection secFltEnv   { "FILTER ENV" };
    WaspSection secLfo1     { "LFO 1" };
    WaspSection secLfo2     { "LFO 2" };
    WaspSection secFx       { "DISTORTION" };
    WaspSection secChar     { "CHARACTER" };
    WaspSection secModEnv   { "MOD ENV" };
    WaspSection secVelocity { "VELOCITY" };
    WaspSection secUnison   { "UNISON" };
    WaspSection secFilter2  { "FILTER 2" };
    WaspSection secRouting  { "FILTER ROUTING" };

    // --- Osc 1 ---
    juce::ComboBox osc1Shape;
    WaspKnob       osc1Coarse { "COARSE", laf };
    WaspKnob       osc1Fine   { "FINE",   laf };

    // --- Osc 2 ---
    juce::ComboBox osc2Shape;
    WaspKnob       osc2Coarse { "COARSE", laf };
    WaspKnob       osc2Fine   { "FINE",   laf };

    // --- Osc 3 ---
    juce::ComboBox osc3Shape;
    WaspKnob       osc3Amount { "AMOUNT", laf };

    // --- Mix / Mod ---
    WaspKnob mixKnob { "OSC MIX", laf };
    WaspKnob pwKnob  { "PW",      laf };
    WaspKnob fmKnob  { "FM",      laf };
    WaspKnob rmKnob  { "RM",      laf };

    // --- Filter ---
    juce::ComboBox fltType;
    WaspKnob       cutoff    { "CUTOFF",   laf };
    WaspKnob       resonance { "RESO",     laf };
    WaspKnob       fltEnvAmt { "ENV AMT",  laf };
    WaspKnob       fltKT     { "KEYTRACK", laf };

    // --- Amp Env ---
    WaspKnob ampA { "ATK",  laf };
    WaspKnob ampD { "DEC",  laf };
    WaspKnob ampS { "SUS",  laf };
    WaspKnob ampR { "REL",  laf };

    // --- Filter Env ---
    WaspKnob fltA { "ATK",  laf };
    WaspKnob fltD { "DEC",  laf };
    WaspKnob fltS { "SUS",  laf };
    WaspKnob fltR { "REL",  laf };

    // --- LFO 1 ---
    juce::ComboBox  lfo1Shape;
    juce::ComboBox  lfo1Target;
    WaspKnob        lfo1Freq   { "FREQ",   laf };
    WaspKnob        lfo1Amount { "AMOUNT", laf };
    WaspKnob        lfo1Delay  { "DELAY",  laf };
    juce::ToggleButton lfo1Sync  { "SYNC"  };
    juce::ToggleButton lfo1Reset { "RESET" };

    // --- LFO 2 ---
    juce::ComboBox  lfo2Shape;
    juce::ComboBox  lfo2Target;
    WaspKnob        lfo2Freq   { "FREQ",   laf };
    WaspKnob        lfo2Amount { "AMOUNT", laf };
    WaspKnob        lfo2Delay  { "DELAY",  laf };
    juce::ToggleButton lfo2Sync  { "SYNC"  };
    juce::ToggleButton lfo2Reset { "RESET" };

    // --- Distortion ---
    juce::ToggleButton distOn    { "ON"    };
    WaspKnob           distDrive { "DRIVE", laf };
    WaspKnob           distTone  { "TONE",  laf };

    // --- Character ---
    WaspKnob           dualVoice { "DUAL VOICE", laf };
    WaspKnob           waspChar  { "WASP",       laf };
    WaspKnob           vcoBinary { "VCO BINARY", laf };
    juce::ToggleButton cmosFilter { "CMOS" };    // CMOS filter mode toggle

    // --- CV (Control Voltage) ---
    WaspSection        secCV     { "CONTROL VOLTAGE" };
    WaspKnob           cvAmount  { "CV AMT", laf };
    juce::ComboBox     cvTarget;

    // --- Mod Env (one-shot AD generator) ---
    WaspKnob        modEnvAtk { "ATK", laf };
    WaspKnob        modEnvDec { "DEC", laf };
    WaspKnob        modEnvAmt { "AMT", laf };
    juce::ComboBox  modEnvDest;

    // --- Velocity ---
    WaspKnob        velToAmp { "AMP",    laf };
    WaspKnob        velToFlt { "FILTER", laf };

    // --- Dual voice detune (moved next to CHARACTER's dual voice knob) ---
    WaspKnob        dualDetune { "DETUNE", laf };

    // --- Filter 2 (KillerBee dual-filter addition) ---
    juce::ComboBox flt2Type;
    WaspKnob       cutoff2    { "CUTOFF 2", laf };
    WaspKnob       resonance2 { "RESO 2",   laf };
    WaspKnob       flt2EnvAmt { "ENV AMT 2",laf };
    WaspKnob       flt2KT     { "KEYTRK 2", laf };

    // --- Filter routing ---
    juce::ComboBox fltRouting;
    WaspKnob       fltParallelMix { "1<->2 MIX", laf };

    // --- Delay (KillerBee addition) ---
    WaspSection secDelay { "DELAY" };
    WaspKnob    delayTime     { "TIME",     laf };
    WaspKnob    delayFeedback { "FEEDBACK", laf };
    WaspKnob    delayMix      { "MIX",      laf };

    // --- Output (Master Volume) ---
    WaspSection secOutput { "OUTPUT" };
    WaspKnob    masterVolume { "VOLUME", laf };

    // --- APVTS attachments ---
    using SliderAtt = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboAtt  = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using ButtonAtt = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<ComboAtt>  attOsc1Shape, attOsc2Shape, attOsc3Shape;
    std::unique_ptr<SliderAtt> attOsc1Coarse, attOsc1Fine;
    std::unique_ptr<SliderAtt> attOsc2Coarse, attOsc2Fine;
    std::unique_ptr<SliderAtt> attOsc3Amount;
    std::unique_ptr<SliderAtt> attMix, attPW, attFM, attRM;
    std::unique_ptr<ComboAtt>  attFltType;
    std::unique_ptr<SliderAtt> attCutoff, attReso, attFltEnvAmt, attFltKT;
    std::unique_ptr<SliderAtt> attAmpA, attAmpD, attAmpS, attAmpR;
    std::unique_ptr<SliderAtt> attFltA, attFltD, attFltS, attFltR;
    std::unique_ptr<ComboAtt>  attLfo1Shape, attLfo1Target;
    std::unique_ptr<SliderAtt> attLfo1Freq, attLfo1Amt, attLfo1Delay;
    std::unique_ptr<ButtonAtt> attLfo1Sync, attLfo1Reset;
    std::unique_ptr<ComboAtt>  attLfo2Shape, attLfo2Target;
    std::unique_ptr<SliderAtt> attLfo2Freq, attLfo2Amt, attLfo2Delay;
    std::unique_ptr<ButtonAtt> attLfo2Sync, attLfo2Reset;
    std::unique_ptr<ButtonAtt> attDistOn;
    std::unique_ptr<SliderAtt> attDistDrive, attDistTone;
    std::unique_ptr<SliderAtt> attDualVoice, attWaspChar, attVCOBinary, attDualDetune;
    std::unique_ptr<ButtonAtt> attCmosFilter;
    std::unique_ptr<SliderAtt> attCVAmount;
    std::unique_ptr<ComboAtt>  attCVTarget;
    std::unique_ptr<SliderAtt> attModEnvAtk, attModEnvDec, attModEnvAmt;
    std::unique_ptr<ComboAtt>  attModEnvDest;
    std::unique_ptr<SliderAtt> attVelToAmp, attVelToFlt;
    std::unique_ptr<ComboAtt>  attFlt2Type, attFltRouting;
    std::unique_ptr<SliderAtt> attCutoff2, attReso2, attFlt2EnvAmt, attFlt2KT, attFltParallelMix;
    std::unique_ptr<SliderAtt> attDelayTime, attDelayFeedback, attDelayMix;
    std::unique_ptr<SliderAtt> attMasterVolume;

    void setupCombo (juce::ComboBox& box, const juce::StringArray& items);
    void layoutKnobRow (juce::Rectangle<int> area, std::initializer_list<WaspKnob*> knobs);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WASPAlphaAudioProcessorEditor)
};
