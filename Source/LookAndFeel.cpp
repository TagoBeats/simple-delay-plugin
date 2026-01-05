/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 14 Aug 2025 11:36:53am
    Author:  Robin

  ==============================================================================
*/

#include "LookAndFeel.h"

const juce::Typeface::Ptr Fonts::typeface =                     //erstellt typface variable
juce::Typeface::createSystemTypefaceFor(                        //lädt die font file
    BinaryData::LatoMedium_ttf, BinaryData::LatoMedium_ttfSize);

juce::Font Fonts::getFont(float height)                     //returning font based on typeface with particular height
{
    return juce::FontOptions(typeface)
                .withMetricsKind(juce::TypefaceMetricsKind::legacy)
                .withHeight(height);
}

juce::Font RotaryKnobLookAndFeel::getLabelFont([[maybe_unused]] juce::Label& label)
{
    return Fonts::getFont();
}

//==========================================================================================

MainLookAndFeel::MainLookAndFeel()
{
    setColour(juce::GroupComponent::textColourId, Colors::Group::label);
    setColour(juce::GroupComponent::outlineColourId, Colors::Group::outline);

}
juce::Font MainLookAndFeel::getLabelFont([[maybe_unused]] juce::Label& label)
{
    return Fonts::getFont();
}

//==========================================================================================

RotaryKnobLookAndFeel::RotaryKnobLookAndFeel()
{
    setColour(juce::Label::textColourId, Colors::Knob::label);      //label above knob, any  juce::Label with textColourID uses color from Colors::Knob::label
    setColour(juce::Slider::textBoxTextColourId, Colors::Knob::label);      //text below knob, any juce::Slider with textBox...
}

void RotaryKnobLookAndFeel::drawRotarySlider(
 juce::Graphics &g,
 int x, int y, int width, [[maybe_unused]] int height,      //unused da circle width und height das selbe
 float sliderPos,
 float rotaryStartAngle, float rotaryEndAngle,
 juce::Slider &slider)
   
{
    auto bounds = juce::Rectangle<int>(x, y, width, width).toFloat();   //.toFloat sets rectangle to float type
    auto knobRect = bounds.reduced(10.0f, 10.0f);       //10 pixel margin
    auto path = juce::Path();
    
    path.addEllipse(knobRect); //add circle to path using dimension of knobRect
    dropShadow.drawForPath(g, path);
    
    g.setColour(Colors::Knob::outline);
    g.fillEllipse(knobRect);
    
    auto innerRect = knobRect.reduced(2.0f, 2.0f);      //takes square from knob and substracts two pixels all sides
    auto gradient = juce::ColourGradient(
                                         Colors::Knob::gradientTop, 0.0f, innerRect.getY(),
                                         Colors::Knob::gradientBottom, 0.0f, innerRect.getBottom(), false);
    g.setGradientFill(gradient);
    g.fillEllipse(innerRect);

    auto center = bounds.getCentre();
    auto radius = bounds.getWidth() / 2.0f;
    auto lineWidth = 3.0f;
    auto arcRadius = radius - lineWidth / 2.0f;
    
    juce::Path backroundArc;
    backroundArc.addCentredArc(center.x, center.y, arcRadius, arcRadius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
    auto strokeType = juce::PathStrokeType(
     lineWidth, juce::PathStrokeType::curved,
     juce::PathStrokeType::rounded);
    g.setColour(Colors::Knob::trackBackground);
    g.strokePath(backroundArc, strokeType);
    
    auto dialRadius = innerRect.getHeight() / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    
    juce::Point<float> dialStart(center.x + 10.0f * std::sin(toAngle),  //erstellt knob zeiger, wird mit dialstart dialende und dial radius definiert
                                 center.y - 10.0f * std::cos(toAngle));
    juce::Point<float> dialEnd(center.x + dialRadius * std::sin(toAngle),
                               center.y - dialRadius * std::cos(toAngle));
    
    juce::Path dialPath;
    dialPath.startNewSubPath(dialStart);
    dialPath.lineTo(dialEnd);
    g.setColour(Colors::Knob::dial);
    g.strokePath(dialPath, strokeType);
    
    if (slider.isEnabled()) {
        float fromAngle = rotaryStartAngle;
            if (slider.getProperties()["drawFromMiddle"]) {
                fromAngle += (rotaryEndAngle - rotaryStartAngle) / 2.0f;
            }
        juce::Path valueArc;
        valueArc.addCentredArc(center.x,
                               center.y,
                               arcRadius,
                               arcRadius,
                               0.0f,
                               fromAngle,
                               toAngle,
                               true);
        
        g.setColour(slider.findColour(juce::Slider::rotarySliderFillColourId));
        g.strokePath(valueArc, strokeType);
        
        setColour(juce::Slider::rotarySliderFillColourId, Colors::Knob::trackActive);
        }
    
   
    
    }

ButtonLookAndFeel::ButtonLookAndFeel()
{
    setColour(juce::TextButton::textColourOffId, Colors::Button::text);
        setColour(juce::TextButton::textColourOnId, Colors::Button::textToggled);
        setColour(juce::TextButton::buttonColourId, Colors::Button::background);
        setColour(juce::TextButton::buttonOnColourId,
                  Colors::Button::backgroundToggled);
}

void ButtonLookAndFeel::drawButtonBackground(
    juce::Graphics& g,
    juce::Button& button,
    const juce::Colour& backgroundColour,
    [[maybe_unused]] bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat();
    auto cornerSize = bounds.getHeight() * 0.25f;
    auto buttonRect = bounds.reduced(1.0f, 1.0f).withTrimmedBottom(1.0f);

    if (shouldDrawButtonAsDown) {
        buttonRect.translate(0.0f, 1.0f);
    }

    g.setColour(backgroundColour);
    g.fillRoundedRectangle(buttonRect, cornerSize);

    g.setColour(Colors::Button::outline);
    g.drawRoundedRectangle(buttonRect, cornerSize, 2.0f);
}

void ButtonLookAndFeel::drawButtonText(
    juce::Graphics& g,
    juce::TextButton& button,
    [[maybe_unused]] bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat();
    auto buttonRect = bounds.reduced(1.0f, 1.0f).withTrimmedBottom(1.0f);

    if (shouldDrawButtonAsDown) {
        buttonRect.translate(0.0f, 1.0f);
    }

    if (button.getToggleState()) {
        g.setColour(button.findColour(juce::TextButton::textColourOnId));
    } else {
        g.setColour(button.findColour(juce::TextButton::textColourOffId));
    }

    g.setFont(Fonts::getFont());
    g.drawText(button.getButtonText(), buttonRect, juce::Justification::centred);
}
