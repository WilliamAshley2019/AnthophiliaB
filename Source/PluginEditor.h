#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
// KillerBee Look and Feel — Concept 1: Analog Obsession "Mastering Hardware"
//==============================================================================
class WaspLookAndFeel : public juce::LookAndFeel_V4
{
public:
    // ---- Body / global ----
    static constexpr juce::uint32 COL_BODY_BG      = 0xff0e0806u;  // near-black warm
    static constexpr juce::uint32 COL_WASP_STRIPE  = 0xff281008u;  // 1px ribbed pinstripe

    // ---- Alternating panel tones (U-He style) ----
    static constexpr juce::uint32 COL_PANEL_A      = 0xff120d0bu;  // Dark Slate Charcoal
    static constexpr juce::uint32 COL_PANEL_B      = 0xff1c0e08u;  // Warm Mahogany Brown
    static constexpr juce::uint32 COL_PANEL_C      = 0xff240a05u;  // Deep Burnt Amber

    // ---- Section header ribbon (inverted: dark bar, bright gradient text) ----
    static constexpr juce::uint32 COL_SECTION_HDR  = 0xff222222u;
    static constexpr juce::uint32 COL_HDR_TEXT_A   = 0xffff8c00u;  // orange
    static constexpr juce::uint32 COL_HDR_TEXT_B   = 0xffe0281au;  // red

    // ---- Accents ----
    static constexpr juce::uint32 COL_WASP_YELLOW  = 0xffff7a00u;
    static constexpr juce::uint32 COL_WASP_AMBER   = 0xffe0281au;
    static constexpr juce::uint32 COL_GRAD_ORANGE  = 0xffff8c00u;
    static constexpr juce::uint32 COL_GRAD_RED     = 0xffcc1100u;

    // ---- Knob (Monark-style 3D) ----
    static constexpr juce::uint32 COL_KNOB_BODY_HI = 0xff4a4a4au;  // brushed aluminum highlight
    static constexpr juce::uint32 COL_KNOB_BODY_LO = 0xff101010u;  // shadowed base
    static constexpr juce::uint32 COL_KNOB_RING    = 0xff2a1008u;  // outer track
    static constexpr juce::uint32 COL_KNOB_MARKER  = 0xffff8800u;  // glowing orange marker
    static constexpr juce::uint32 COL_OUTLINE      = 0xff4a1c10u;

    // ---- Text ----
    static constexpr juce::uint32 COL_TEXT_PRIMARY   = 0xffffa040u;
    static constexpr juce::uint32 COL_TEXT_SECONDARY = 0xffb06030u;

    // ---- Buttons / combos ----
    static constexpr juce::uint32 COL_BUTTON_OFF = 0xff180808u;
    static constexpr juce::uint32 COL_BUTTON_ON  = 0xffff4400u;
    static constexpr juce::uint32 COL_COMBO_BG   = 0xff0a0505u;

    WaspLookAndFeel()
    {
        setColour (juce::Slider::thumbColourId,               juce::Colour (COL_KNOB_MARKER));
        setColour (juce::Slider::rotarySliderFillColourId,    juce::Colour (COL_WASP_AMBER));
        setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colour (COL_OUTLINE));
        setColour (juce::Slider::trackColourId,               juce::Colour (COL_OUTLINE));
        setColour (juce::Label::textColourId,                 juce::Colour (COL_TEXT_PRIMARY));
        setColour (juce::ComboBox::backgroundColourId,        juce::Colour (COL_COMBO_BG));
        setColour (juce::ComboBox::textColourId,              juce::Colour (COL_TEXT_PRIMARY));
        setColour (juce::ComboBox::outlineColourId,           juce::Colour (COL_OUTLINE));
        setColour (juce::ComboBox::arrowColourId,             juce::Colour (COL_WASP_YELLOW));
        setColour (juce::PopupMenu::backgroundColourId,       juce::Colour (COL_PANEL_A));
        setColour (juce::PopupMenu::textColourId,             juce::Colour (COL_TEXT_PRIMARY));
        setColour (juce::PopupMenu::highlightedBackgroundColourId, juce::Colour (COL_WASP_YELLOW));
        setColour (juce::PopupMenu::highlightedTextColourId,  juce::Colour (0xff000000u));
        setColour (juce::ToggleButton::textColourId,          juce::Colour (COL_TEXT_PRIMARY));
        setColour (juce::ToggleButton::tickColourId,          juce::Colour (COL_WASP_YELLOW));
        setColour (juce::ToggleButton::tickDisabledColourId,  juce::Colour (COL_OUTLINE));
    }

    //==========================================================================
    // Monark-style 3D rotary knob
    //==========================================================================
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, juce::Slider& slider) override
    {
        juce::ignoreUnused (slider);

        auto bounds = juce::Rectangle<float> ((float) x, (float) y,
                                              (float) width, (float) height).reduced (4.0f);
        const float radius    = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
        const float centreX   = bounds.getCentreX();
        const float centreY   = bounds.getCentreY();
        const float angle     = rotaryStartAngle
                              + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        // --- Outer indicator ring track ---
        const float ringRadius = radius + 3.0f;
        {
            juce::Path track;
            track.addCentredArc (centreX, centreY, ringRadius, ringRadius, 0.0f,
                                 rotaryStartAngle, rotaryEndAngle, true);
            g.setColour (juce::Colour (COL_KNOB_RING));
            g.strokePath (track, juce::PathStrokeType (2.0f));
        }

        // --- Value arc (orange fill from start to current angle) ---
        {
            juce::Path valueArc;
            valueArc.addCentredArc (centreX, centreY, ringRadius, ringRadius, 0.0f,
                                    rotaryStartAngle, angle, true);
            juce::ColourGradient arcGrad (
                juce::Colour (COL_GRAD_ORANGE),
                centreX + ringRadius * std::sin (rotaryStartAngle),
                centreY - ringRadius * std::cos (rotaryStartAngle),
                juce::Colour (COL_GRAD_RED),
                centreX + ringRadius * std::sin (rotaryEndAngle),
                centreY - ringRadius * std::cos (rotaryEndAngle),
                false);
            g.setGradientFill (arcGrad);
            g.strokePath (valueArc, juce::PathStrokeType (2.75f,
                            juce::PathStrokeType::curved,
                            juce::PathStrokeType::rounded));
        }

        // --- Heavy drop-shadow under the knob cap rim (3D float) ---
        g.setColour (juce::Colour (0xaa000000u));
        g.fillEllipse (centreX - radius + 2.0f, centreY - radius + 4.0f,
                       radius * 2.0f, radius * 2.0f);

        // --- Brushed-aluminum knob cap ---
        {
            juce::ColourGradient bodyGrad (
                juce::Colour (COL_KNOB_BODY_HI),
                centreX - radius * 0.35f, centreY - radius * 0.45f,
                juce::Colour (COL_KNOB_BODY_LO),
                centreX + radius * 0.55f, centreY + radius * 0.55f,
                true);
            bodyGrad.addColour (0.55, juce::Colour (0xff2a2a2au));
            g.setGradientFill (bodyGrad);
            g.fillEllipse (centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f);
        }

        // --- Fine concentric brushed texture (a couple of thin rings) ---
        g.setColour (juce::Colours::white.withAlpha (0.035f));
        g.drawEllipse (centreX - radius * 0.78f, centreY - radius * 0.78f,
                       radius * 1.56f, radius * 1.56f, 0.6f);
        g.setColour (juce::Colours::black.withAlpha (0.28f));
        g.drawEllipse (centreX - radius * 0.55f, centreY - radius * 0.55f,
                       radius * 1.10f, radius * 1.10f, 0.6f);

        // --- Sharp black perimeter ring ---
        g.setColour (juce::Colours::black);
        g.drawEllipse (centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f, 1.5f);

        // --- Thin bright highlight just inside the perimeter ---
        g.setColour (juce::Colour (COL_WASP_YELLOW).withAlpha (0.35f));
        g.drawEllipse (centreX - radius + 1.5f, centreY - radius + 1.5f,
                       (radius - 1.5f) * 2.0f, (radius - 1.5f) * 2.0f, 0.7f);

        // --- Glowing orange marker line cut into the top of the cap ---
        {
            const float innerR = radius * 0.42f;
            const float outerR = radius * 0.92f;
            const float sx = centreX + innerR * std::sin (angle);
            const float sy = centreY - innerR * std::cos (angle);
            const float ex = centreX + outerR * std::sin (angle);
            const float ey = centreY - outerR * std::cos (angle);

            // Outer glow
            g.setColour (juce::Colour (COL_KNOB_MARKER).withAlpha (0.35f));
            g.drawLine (sx, sy, ex, ey, 4.0f);
            // Bright core
            g.setColour (juce::Colour (COL_KNOB_MARKER));
            g.drawLine (sx, sy, ex, ey, 1.8f);
        }

        // --- Center notch dot (small dark well) ---
        g.setColour (juce::Colour (0xff050505u));
        g.fillEllipse (centreX - 2.5f, centreY - 2.5f, 5.0f, 5.0f);
        g.setColour (juce::Colour (0x60ffffffu));
        g.drawEllipse (centreX - 2.5f, centreY - 2.5f, 5.0f, 5.0f, 0.6f);
    }

    //==========================================================================
    // LED toggle
    //==========================================================================
    void drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                           bool shouldDrawButtonAsHighlighted,
                           bool shouldDrawButtonAsDown) override
    {
        juce::ignoreUnused (shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

        auto bounds = button.getLocalBounds().toFloat().reduced (1.0f);
        const bool toggled = button.getToggleState();

        const float ledSize = juce::jmin (bounds.getHeight() - 4.0f, 12.0f);
        const float ledX    = bounds.getX() + 2.0f;
        const float ledY    = bounds.getCentreY() - ledSize * 0.5f;

        // Recessed metallic bezel
        g.setColour (juce::Colour (0xff050202u));
        g.fillEllipse (ledX - 1.5f, ledY - 1.5f, ledSize + 3.0f, ledSize + 3.0f);

        // LED body
        g.setColour (toggled ? juce::Colour (COL_WASP_YELLOW)
                             : juce::Colour (COL_BUTTON_OFF));
        g.fillEllipse (ledX, ledY, ledSize, ledSize);

        // Black rim
        g.setColour (juce::Colours::black);
        g.drawEllipse (ledX, ledY, ledSize, ledSize, 1.0f);

        if (toggled)
        {
            juce::ColourGradient glow (
                juce::Colour (COL_GRAD_ORANGE).withAlpha (0.55f),
                ledX + ledSize * 0.5f, ledY + ledSize * 0.5f,
                juce::Colour (COL_GRAD_RED).withAlpha (0.0f),
                ledX - 3.0f, ledY - 3.0f, true);
            g.setGradientFill (glow);
            g.fillEllipse (ledX - 4.0f, ledY - 4.0f, ledSize + 8.0f, ledSize + 8.0f);
        }

        g.setColour (juce::Colour (toggled ? COL_TEXT_PRIMARY : COL_TEXT_SECONDARY));
        g.setFont (juce::FontOptions ("Courier New", 10.0f, juce::Font::bold));
        g.drawText (button.getButtonText(),
                    (int)(ledX + ledSize + 4.0f), (int) bounds.getY(),
                    (int)(bounds.getWidth() - ledSize - 8.0f), (int) bounds.getHeight(),
                    juce::Justification::centredLeft, false);
    }

    juce::Font getLabelFont (juce::Label& label) override
    {
        juce::ignoreUnused (label);
        return juce::FontOptions ("Courier New", 10.0f, juce::Font::bold);
    }

    //==========================================================================
    // Combo box
    //==========================================================================
    void drawComboBox (juce::Graphics& g, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       juce::ComboBox& box) override
    {
        juce::ignoreUnused (isButtonDown, box);

        auto bounds = juce::Rectangle<int> (0, 0, width, height).toFloat();

        g.setColour (juce::Colour (COL_COMBO_BG));
        g.fillRoundedRectangle (bounds, 2.0f);

        // Orange→red gradient underline
        juce::ColourGradient underline (
            juce::Colour (COL_GRAD_ORANGE), bounds.getX(), bounds.getBottom(),
            juce::Colour (COL_GRAD_RED),    bounds.getRight(), bounds.getBottom(), false);
        g.setGradientFill (underline);
        g.fillRect (bounds.getX(), bounds.getBottom() - 1.5f, bounds.getWidth(), 1.5f);

        g.setColour (juce::Colours::black);
        g.drawRoundedRectangle (bounds.reduced (0.5f), 2.0f, 1.0f);

        // Chevron
        juce::Path arrow;
        const float ax = (float) (buttonX + buttonW / 2);
        const float ay = (float) (buttonY + buttonH / 2 - 2);
        arrow.addTriangle (ax - 4.0f, ay, ax + 4.0f, ay, ax, ay + 5.0f);
        g.setColour (juce::Colour (COL_WASP_YELLOW));
        g.fillPath (arrow);
    }
};

//==============================================================================
// Labelled rotary knob
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
        label.setColour (juce::Label::textColourId,
                         juce::Colour (WaspLookAndFeel::COL_TEXT_SECONDARY));
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
        const int labelH = 14;
        label.setBounds (b.removeFromBottom (labelH));
        slider.setBounds (b);
    }
};

//==============================================================================
// Section panel — U-He style alternating tones, inverted header ribbon,
// ribbed pinstripe texture behind modules
//==============================================================================
class WaspSection : public juce::Component
{
public:
    juce::String title;
    int          paletteIndex = 0; // 0=A, 1=B, 2=C

    WaspSection (const juce::String& t) : title (t) {}

    void setPaletteIndex (int idx) { paletteIndex = ((idx % 3) + 3) % 3; repaint(); }

    static juce::Colour paletteColour (int idx)
    {
        switch (((idx % 3) + 3) % 3)
        {
            case 1:  return juce::Colour (WaspLookAndFeel::COL_PANEL_B);
            case 2:  return juce::Colour (WaspLookAndFeel::COL_PANEL_C);
            default: return juce::Colour (WaspLookAndFeel::COL_PANEL_A);
        }
    }

    void paint (juce::Graphics& g) override
    {
        auto b = getLocalBounds().toFloat();

        // --- Panel background (alternating tone) ---
        g.setColour (paletteColour (paletteIndex));
        g.fillRoundedRectangle (b, 3.0f);

        // --- Ribbed pinstripe texture behind the whole module (EDP Wasp homage) ---
        {
            juce::Graphics::ScopedSaveState save (g);
            juce::Path clip;
            clip.addRoundedRectangle (b, 3.0f);
            g.reduceClipRegion (clip);

            g.setColour (juce::Colour (WaspLookAndFeel::COL_WASP_STRIPE).withAlpha (0.55f));
            for (float y = b.getY() + 18.0f; y < b.getBottom(); y += 3.0f)
                g.fillRect (b.getX(), y, b.getWidth(), 1.0f);
        }

        // --- Perimeter: sharp black outer ring + faint orange inner highlight ---
        g.setColour (juce::Colours::black);
        g.drawRoundedRectangle (b.reduced (0.5f), 3.0f, 1.2f);
        g.setColour (juce::Colour (WaspLookAndFeel::COL_WASP_YELLOW).withAlpha (0.18f));
        g.drawRoundedRectangle (b.reduced (2.0f), 2.5f, 0.6f);

        // --- Inverted header ribbon: solid dark bar, bright gradient text ---
        juce::Rectangle<float> header (b.getX(), b.getY(), b.getWidth(), 16.0f);

        // Solid #222222 base
        g.setColour (juce::Colour (WaspLookAndFeel::COL_SECTION_HDR));
        g.fillRoundedRectangle (header, 3.0f);
        g.fillRect (header.withTrimmedTop (4.0f));

        // Very subtle vertical gloss so the bar reads as a physical ribbon
        {
            juce::ColourGradient gloss (
                juce::Colours::white.withAlpha (0.05f), header.getX(), header.getY(),
                juce::Colours::transparentBlack, header.getX(), header.getBottom(), false);
            g.setGradientFill (gloss);
            g.fillRoundedRectangle (header, 3.0f);
            g.fillRect (header.withTrimmedTop (4.0f));
        }

        // Bright orange→red gradient text (inverted from the old look)
        {
            juce::GlyphArrangement glyphs;
            const auto font = juce::Font (juce::FontOptions ("Courier New", 9.5f, juce::Font::bold));
            glyphs.addLineOfText (font, title, 0.0f, 0.0f);

            const auto textArea = header.toNearestInt();
            juce::Rectangle<float> textBounds (
                (float) textArea.getX(), (float) textArea.getY(),
                (float) textArea.getWidth(), (float) textArea.getHeight());

            juce::ColourGradient textGrad (
                juce::Colour (WaspLookAndFeel::COL_HDR_TEXT_A), textBounds.getX(), textBounds.getCentreY(),
                juce::Colour (WaspLookAndFeel::COL_HDR_TEXT_B), textBounds.getRight(), textBounds.getCentreY(),
                false);

            g.setGradientFill (textGrad);
            g.setFont (font);
            g.drawText (title, textArea, juce::Justification::centred, false);
        }

        // Amber accent line under the header
        g.setColour (juce::Colour (WaspLookAndFeel::COL_WASP_AMBER).withAlpha (0.7f));
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
    WaspSection secCV       { "CONTROL VOLTAGE" };
    WaspSection secDelay    { "DELAY" };
    WaspSection secOutput   { "OUTPUT" };

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
    juce::ComboBox fltModel; // Ladder / Chamberlin
    WaspKnob       cutoff    { "CUTOFF",   laf };
    WaspKnob       resonance { "RESO",     laf };
    WaspKnob       fltEnvAmt { "ENV AMT",  laf };
    WaspKnob       fltKT     { "KEYTRACK", laf };

    // --- Amp Env ---
    WaspKnob ampA { "ATK", laf };
    WaspKnob ampD { "DEC", laf };
    WaspKnob ampS { "SUS", laf };
    WaspKnob ampR { "REL", laf };

    // --- Filter Env ---
    WaspKnob fltA { "ATK", laf };
    WaspKnob fltD { "DEC", laf };
    WaspKnob fltS { "SUS", laf };
    WaspKnob fltR { "REL", laf };

    // --- LFO 1 ---
    juce::ComboBox     lfo1Shape;
    juce::ComboBox     lfo1Target;
    WaspKnob           lfo1Freq   { "FREQ",   laf };
    WaspKnob           lfo1Amount { "AMOUNT", laf };
    WaspKnob           lfo1Delay  { "DELAY",  laf };
    juce::ToggleButton lfo1Sync   { "SYNC"  };
    juce::ToggleButton lfo1Reset  { "RESET" };

    // --- LFO 2 ---
    juce::ComboBox     lfo2Shape;
    juce::ComboBox     lfo2Target;
    WaspKnob           lfo2Freq   { "FREQ",   laf };
    WaspKnob           lfo2Amount { "AMOUNT", laf };
    WaspKnob           lfo2Delay  { "DELAY",  laf };
    juce::ToggleButton lfo2Sync   { "SYNC"  };
    juce::ToggleButton lfo2Reset  { "RESET" };

    // --- Distortion ---
    juce::ToggleButton distOn    { "ON"    };
    WaspKnob           distDrive { "DRIVE", laf };
    WaspKnob           distTone  { "TONE",  laf };
    juce::ComboBox     distType; // Soft / Hard

    // --- Character ---
    WaspKnob           dualVoice { "DUAL VOICE", laf };
    WaspKnob           waspChar  { "WASP",       laf };
    WaspKnob           vcoBinary { "VCO BINARY", laf };
    juce::ToggleButton cmosFilter { "CMOS" };

    // --- CV ---
    WaspKnob       cvAmount { "CV AMT", laf };
    juce::ComboBox cvTarget;

    // --- Mod Env ---
    WaspKnob       modEnvAtk  { "ATK", laf };
    WaspKnob       modEnvDec  { "DEC", laf };
    WaspKnob       modEnvAmt  { "AMT", laf };
    juce::ComboBox modEnvDest;

    // --- Velocity ---
    WaspKnob velToAmp { "AMP",    laf };
    WaspKnob velToFlt { "FILTER", laf };

    // --- Unison ---
    WaspKnob dualDetune { "DETUNE", laf };

    // --- Filter 2 ---
    juce::ComboBox flt2Type;
    juce::ComboBox flt2Model; // Ladder / Chamberlin
    WaspKnob       cutoff2    { "CUTOFF 2", laf };
    WaspKnob       resonance2 { "RESO 2",   laf };
    WaspKnob       flt2EnvAmt { "ENV AMT 2",laf };
    WaspKnob       flt2KT     { "KEYTRK 2", laf };

    // --- Filter routing ---
    juce::ComboBox fltRouting;
    WaspKnob       fltParallelMix { "1<->2 MIX", laf };

    // --- Delay ---
    WaspKnob delayTime     { "TIME",     laf };
    WaspKnob delayFeedback { "FEEDBACK", laf };
    WaspKnob delayMix      { "MIX",      laf };

    // --- Output ---
    WaspKnob masterVolume { "VOLUME", laf };

    // --- APVTS attachments ---
    using SliderAtt = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboAtt  = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using ButtonAtt = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<ComboAtt>  attOsc1Shape, attOsc2Shape, attOsc3Shape;
    std::unique_ptr<SliderAtt> attOsc1Coarse, attOsc1Fine;
    std::unique_ptr<SliderAtt> attOsc2Coarse, attOsc2Fine;
    std::unique_ptr<SliderAtt> attOsc3Amount;
    std::unique_ptr<SliderAtt> attMix, attPW, attFM, attRM;
    std::unique_ptr<ComboAtt>  attFltType, attFltModel;
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
    std::unique_ptr<ComboAtt>  attDistType;
    std::unique_ptr<SliderAtt> attDistDrive, attDistTone;
    std::unique_ptr<SliderAtt> attDualVoice, attWaspChar, attVCOBinary, attDualDetune;
    std::unique_ptr<ButtonAtt> attCmosFilter;
    std::unique_ptr<SliderAtt> attCVAmount;
    std::unique_ptr<ComboAtt>  attCVTarget;
    std::unique_ptr<SliderAtt> attModEnvAtk, attModEnvDec, attModEnvAmt;
    std::unique_ptr<ComboAtt>  attModEnvDest;
    std::unique_ptr<SliderAtt> attVelToAmp, attVelToFlt;
    std::unique_ptr<ComboAtt>  attFlt2Type, attFltRouting, attFlt2Model;
    std::unique_ptr<SliderAtt> attCutoff2, attReso2, attFlt2EnvAmt, attFlt2KT, attFltParallelMix;
    std::unique_ptr<SliderAtt> attDelayTime, attDelayFeedback, attDelayMix;
    std::unique_ptr<SliderAtt> attMasterVolume;

    void setupCombo (juce::ComboBox& box, const juce::StringArray& items);
    void assignSectionPalettes();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WASPAlphaAudioProcessorEditor)
};