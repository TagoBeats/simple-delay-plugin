/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayAudioProcessorEditor::DelayAudioProcessorEditor (DelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), meter(p.levelL, p.levelR)
{
    delayGroup.setText("Delay");
    delayGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    delayGroup.addAndMakeVisible(delayTimeKnob);
    delayGroup.addChildComponent(delayNoteKnob);
    addAndMakeVisible(delayGroup);
    
    tempoSyncButton.setButtonText("Sync");
    tempoSyncButton.setClickingTogglesState(true);
    tempoSyncButton.setBounds(0, 0, 70, 27);
    tempoSyncButton.setLookAndFeel(ButtonLookAndFeel::get());
    delayGroup.addAndMakeVisible(tempoSyncButton);

    feedbackGroup.setText("Feedback");
    feedbackGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    feedbackGroup.addAndMakeVisible(feedbackKnob);
    feedbackGroup.addAndMakeVisible(stereowidthKnob);
    feedbackGroup.addAndMakeVisible(lowCutKnob);
    feedbackGroup.addAndMakeVisible(highCutKnob);
    addAndMakeVisible(feedbackGroup);
    
    outputGroup.setText("Output");
    outputGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    outputGroup.addAndMakeVisible(mixKnob);
    outputGroup.addAndMakeVisible(gainKnob);
    addAndMakeVisible(outputGroup);
    
    outputGroup.addAndMakeVisible(meter);
    
    auto bypassIcon =
    juce::ImageCache::getFromMemory(BinaryData::Bypass_png,
                                    BinaryData::Bypass_pngSize);
    bypassButton.setClickingTogglesState(true);
    bypassButton.setBounds(0, 0, 20, 20);
    bypassButton.setImages(
                           false, true, true,
                           bypassIcon, 1.0f, juce::Colours::white,
                           bypassIcon, 1.0f, juce::Colours::white,
                           bypassIcon, 1.0f, juce::Colours::grey,
                           0.0f);
    addAndMakeVisible(bypassButton);
    
    logoGroup.setText("");
    logoGroup.addAndMakeVisible(logoComp);
    addAndMakeVisible(logoGroup);
    
    logoComp.setImage(juce::ImageCache::getFromMemory(BinaryData::Logo_png, BinaryData::Logo_pngSize));
                        
    
    
    setSize (500, 350);
    
    setResizable(true, true);
    setResizeLimits(500, 330, 1000, 660);
    
    setLookAndFeel(&mainLF);
    
    updateDelayKnobs(audioProcessor.params.tempoSyncParam->get());
    audioProcessor.params.tempoSyncParam->addListener(this);

    
}

DelayAudioProcessorEditor::~DelayAudioProcessorEditor()
{
    audioProcessor.params.tempoSyncParam->removeListener(this);
    setLookAndFeel(nullptr);
}

//==============================================================================
void DelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colors::backround);
    gainKnob.slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::rebeccapurple);  // überschreibt die Colour von LookAndFell für gainKnob
    
    auto logoBounds = logoGroup.getBounds().reduced(5);
    //g.drawImageWithin(logo, logoBounds.getX(), logoBounds.getY(), logoBounds.getWidth(), logoBounds.getHeight(), juce::RectanglePlacement::centred);

}

void DelayAudioProcessorEditor::resized()                                               //do layout of components
{
    //auto bounds = getLocalBounds();        Code Lines from the book
    //int height = bounds.getHeight() - 20;
    auto area = getLocalBounds();               // my solution
    
    //int y = 10;
    int spacing = 10;
    const int logoHeight = 60;
    logoGroup.setBounds(10, spacing, area.getWidth() - 20, logoHeight);
    logoComp.setBounds(logoGroup.getLocalBounds().reduced(6));
    
    int yStart = logoGroup.getBottom() + spacing;
    
    bypassButton.setTopLeftPosition(
                area.getRight()- bypassButton.getWidth() - 10, 10);
    
    delayGroup.setBounds(10, yStart, 110, area.getHeight() - 2 * spacing);
    outputGroup.setBounds(area.getWidth() - 160, yStart, 150, area.getHeight() - 2 * spacing);
    feedbackGroup.setBounds(delayGroup.getRight() + spacing, yStart, outputGroup.getX() - 130 - spacing, area.getHeight() - 2 * spacing);
    
    delayTimeKnob.setTopLeftPosition(20, 20);
    tempoSyncButton.setTopLeftPosition(20, delayTimeKnob.getBottom() + 10);
    delayNoteKnob.setTopLeftPosition(delayTimeKnob.getX(), delayTimeKnob.getY());
    
    mixKnob.setTopLeftPosition(20, 20);
    gainKnob.setTopLeftPosition(mixKnob.getX(), mixKnob.getBottom() + 10);
    
    feedbackKnob.setTopLeftPosition(20, 20);
    stereowidthKnob.setTopLeftPosition(feedbackKnob.getRight() + 20, 20);
    lowCutKnob.setTopLeftPosition(feedbackKnob.getX(), feedbackKnob.getBottom() + 20);
    highCutKnob.setTopLeftPosition(stereowidthKnob.getX(), feedbackKnob.getBottom() + 20);
    //    saturationKnob.setTopLeftPosition(feedbackKnob.getX(), lowCutKnob.getBottom() + 20);
    
    meter.setBounds(outputGroup.getWidth() - 45, 30, 30, gainKnob.getBottom() - 30);
    
    
    
    
    
   
    
}

void DelayAudioProcessorEditor::parameterValueChanged(int, float value)
{
     if (juce::MessageManager::getInstance()->isThisTheMessageThread()) {
            updateDelayKnobs(value != 0.0f);
        } else {
            juce::MessageManager::callAsync([this, value]
            {
                updateDelayKnobs(value != 0.0f);
            });
        }

    DBG("parameter changed: " << value);
}

void DelayAudioProcessorEditor::updateDelayKnobs(bool tempoSyncActive)
{
    delayTimeKnob.setVisible(!tempoSyncActive);
    delayNoteKnob.setVisible(tempoSyncActive);
}


