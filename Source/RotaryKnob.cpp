/*
  ==============================================================================

    RotaryKnob.cpp
    Created: 13 Aug 2025 10:31:57am
    Author:  Robin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RotaryKnob.h"
#include "LookAndFeel.h"
//==============================================================================
RotaryKnob::RotaryKnob(const juce::String& text,
                        juce::AudioProcessorValueTreeState& apvts,
                        const juce::ParameterID& parameterID,
                        bool drawFromMiddle)
     : attachment(apvts, parameterID.getParamID(), slider)

{
    slider.getProperties().set("drawFromMiddle", drawFromMiddle);
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);     //knob size 70x70 + 16 Text = 86 Pixel
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
    slider.setBounds(0, 0, 70, 86);                                                     //Places Slider in top left corner at (0,0)
    addAndMakeVisible(slider);                                                          //makes slider drawn and a child component of editor
    
    label.setText(text, juce::NotificationType::dontSendNotification);                //Labels "Gain", dontSendNotification means text wont sent a notification to any listener
    label.setJustificationType(juce::Justification::horizontallyCentred);
    label.setBorderSize(juce::BorderSize<int>{0,0,2,0});                                //puts 2 Picels of extra spacing between bottom of label and top of knob
    label.attachToComponent(&slider, false);
    addAndMakeVisible(label);
    
    setLookAndFeel(RotaryKnobLookAndFeel::get());
    
    float pi = juce::MathConstants<float>::pi;
    slider.setRotaryParameters(1.25f * pi, 2.75 * pi, true);
    
    setSize(70, 110);                                                                   //without size is 0, 70x86 pixels plus 24 pixels high = 86+24=110

}

RotaryKnob::~RotaryKnob()
{
}



void RotaryKnob::resized()
{
    slider.setTopLeftPosition(0, 24);
}
