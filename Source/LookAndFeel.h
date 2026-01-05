/*
  ==============================================================================

    LookAndFeel.h
    Created: 14 Aug 2025 11:36:53am
    Author:  Robin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace Colors {
const juce::Colour backround {245, 240, 235};
const juce::Colour header {40, 40, 40,};

        namespace  Knob
        {
            const juce::Colour trackActive { 177, 101, 135 };
            const juce::Colour outline { 255, 250, 245 };
            const juce::Colour gradientTop { 250, 245, 240 };
            const juce::Colour gradientBottom { 240, 235, 230 };
            const juce::Colour dial { 100, 100, 100 };
            const juce::Colour dropShadow { 195, 190, 185 };
            const juce::Colour label { 80, 80, 80 };
            const juce::Colour textBoxBackground { 80, 80, 80 };
            const juce::Colour value { 240, 240, 240 };
            const juce::Colour caret { 255, 255, 255 };
            const juce::Colour trackBackground { 205, 200, 195 };
        }
        namespace Group
        {
            const juce::Colour label { 160, 155, 150};
            const juce::Colour outline { 235, 230, 255};
        }
        namespace Button
    {
            const juce::Colour text { 80, 80, 80 };
            const juce::Colour textToggled { 40, 40, 40 };
            const juce::Colour background { 245, 240, 235 };
            const juce::Colour backgroundToggled { 255, 250, 245 };
            const juce::Colour outline { 235, 230, 225 };
    }
        namespace LevelMeter
    {
            const juce::Colour background { 245, 240, 235 };
            const juce::Colour tickLine { 200, 200, 200 };
            const juce::Colour tickLabel { 80, 80, 80 };
            const juce::Colour tooLoud { 226, 74, 81 };
            const juce::Colour levelOK { 65, 206, 88 };
    }
        

}
//=====================================================================
class Fonts
{
public:
    static juce::Font getFont(float height = 16.0f);
    Fonts() = delete;

private:
    static const juce::Typeface::Ptr typeface;
};
//=====================================================================

class RotaryKnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
    RotaryKnobLookAndFeel();
    static RotaryKnobLookAndFeel* get()         //erstellt pointer auf dem wir aus gesamten code zugreifen können
    {
        static RotaryKnobLookAndFeel instance;
        return &instance;
    }
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                              float sliderPos, float rotaryStartAngle,
                              float rotaryEndAngle, juce::Slider& slider) override;     //wendet drawRotarySlider auf juce::Slider function an
    
    juce::Font getLabelFont(juce::Label&) override;
    
private:
    juce::DropShadow dropShadow {Colors::Knob::dropShadow, 6, { 0, 3 } };    //creates new juce::Dropshadow object mit colour, radius (6) and offset (0 hor, 3 vert)
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotaryKnobLookAndFeel)
};

//=====================================================================

class MainLookAndFeel : public juce::LookAndFeel_V4
{
public:
    MainLookAndFeel();
    juce::Font getLabelFont(juce::Label&) override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainLookAndFeel)
};

class ButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    ButtonLookAndFeel();
    
    static ButtonLookAndFeel* get()
    {
        static ButtonLookAndFeel instance;
        return &instance;
    }
    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                        const juce::Colour& backgroundColour,
                        bool shouldDrawButtonAsHighlighted,
                        bool shouldDrawButtonAsDown) override;

    void drawButtonText(juce::Graphics& g, juce::TextButton& button,
                        bool shouldDrawButtonAsHighlighted,
                        bool shouldDrawButtonAsDown) override;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ButtonLookAndFeel)
};
