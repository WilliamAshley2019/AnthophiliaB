#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
// WASPAlpha Look and Feel
// Inspired by the EDP WASP (1978) - iconic yellow & black panel
//==============================================================================
class WaspLookAndFeel : public juce::LookAndFeel_V4
{
public:
    // Palette
    static constexpr juce::uint32 COL_BODY_BG      = 0xff1a1a00u;  // near-black with warm yellow tint
    static constexpr juce::uint32 COL_PANEL_BG      = 0xff212100u;  // panel sections
    static constexpr juce::uint32 COL_SECTION_HDR   = 0xff2e2e00u;  // section header bars
    static constexpr juce::uint32 COL_WASP_YELLOW   = 0xffffcc00u;  // iconic EDP WASP yellow
    static constexpr juce::uint32 COL_WASP_AMBER    = 0xffff9900u;  // amber accent
    static constexpr juce::uint32 COL_KNOB_BODY     = 0xff2a2a08u;  // knob body dark olive
    static constexpr juce::uint32 COL_KNOB_RING     = 0xff444400u;  // knob ring
    static constexpr juce::uint32 COL_KNOB_DOT      = 0xffffcc00u;  // position indicator
    static constexpr juce::uint32 COL_TEXT_PRIMARY   = 0xffffcc00u;  // yellow text
    static constexpr juce::uint32 COL_TEXT_SECONDARY = 0xffaa9900u;  // dimmer labels
    static constexpr juce::uint32 COL_OUTLINE        = 0xff555500u;  // border outlines
    static constexpr juce::uint32 COL_BUTTON_OFF     = 0xff1e1e00u;
    static constexpr juce::uint32 COL_BUTTON_ON      = 0xffffcc00u;
    static constexpr juce::uint32 COL_WASP_STRIPE    = 0xff333300u;  // subtle stripes

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
        g.setColour (juce::Colour (0x55000000u));
        g.fillEllipse (centreX - radius + 2.0f, centreY - radius + 3.0f, radius * 2.0f, radius * 2.0f);

        // Knob body
        juce::ColourGradient bodyGrad (juce::Colour (0xff3a3a10u), centreX - radius * 0.3f, centreY - radius * 0.3f,
                                        juce::Colour (0xff141400u), centreX + radius * 0.5f, centreY + radius * 0.5f,
                                        true);
        g.setGradientFill (bodyGrad);
        g.fillEllipse (centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f);

        // Ring arc (track)
        juce::Path trackArc;
        float trackRadius = radius + 3.0f;
        trackArc.addCentredArc (centreX, centreY, trackRadius, trackRadius, 0.0f,
                                 rotaryStartAngle, rotaryEndAngle, true);
        g.setColour (juce::Colour (COL_OUTLINE));
        g.strokePath (trackArc, juce::PathStrokeType (2.0f));

        // Value arc
        juce::Path valueArc;
        valueArc.addCentredArc (centreX, centreY, trackRadius, trackRadius, 0.0f,
                                 rotaryStartAngle, angle, true);
        g.setColour (juce::Colour (COL_WASP_AMBER));
        g.strokePath (valueArc, juce::PathStrokeType (2.5f));

        // Outer ring
        g.setColour (juce::Colour (COL_OUTLINE));
        g.drawEllipse (centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f, 1.5f);

        // Indicator dot
        float dotDist = radius - 5.0f;
        float dotX    = centreX + dotDist * std::sin (angle);
        float dotY    = centreY - dotDist * std::cos (angle);
        g.setColour (juce::Colour (COL_WASP_YELLOW));
        g.fillEllipse (dotX - 3.0f, dotY - 3.0f, 6.0f, 6.0f);

        // Centre dot
        g.setColour (juce::Colour (0xff555522u));
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
        g.setColour (juce::Colour (COL_OUTLINE));
        g.drawEllipse (ledX, ledY, ledSize, ledSize, 1.0f);

        if (toggled)
        {
            // Glow
            g.setColour (juce::Colour (COL_WASP_YELLOW).withAlpha (0.3f));
            g.fillEllipse (ledX - 2.0f, ledY - 2.0f, ledSize + 4.0f, ledSize + 4.0f);
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
        g.setColour (juce::Colour (COL_KNOB_BODY));
        g.fillRoundedRectangle (bounds, 2.0f);
        g.setColour (juce::Colour (COL_OUTLINE));
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

        // Background
        g.setColour (juce::Colour (WaspLookAndFeel::COL_PANEL_BG));
        g.fillRoundedRectangle (b, 3.0f);

        // Border
        g.setColour (juce::Colour (WaspLookAndFeel::COL_OUTLINE));
        g.drawRoundedRectangle (b.reduced (0.5f), 3.0f, 1.0f);

        // Header bar
        juce::Rectangle<float> header (b.getX(), b.getY(), b.getWidth(), 16.0f);
        g.setColour (juce::Colour (WaspLookAndFeel::COL_SECTION_HDR));
        g.fillRoundedRectangle (header, 3.0f);
        g.fillRect (header.withTrimmedTop (3.0f));

        // Title text
        g.setColour (juce::Colour (WaspLookAndFeel::COL_WASP_YELLOW));
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
    static constexpr int kWindowH = 660;

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

    void setupCombo (juce::ComboBox& box, const juce::StringArray& items);
    void layoutKnobRow (juce::Rectangle<int> area, std::initializer_list<WaspKnob*> knobs);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WASPAlphaAudioProcessorEditor)
};
