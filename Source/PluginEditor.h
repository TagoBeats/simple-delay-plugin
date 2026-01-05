/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"
#include "RotaryKnob.h"
#include "LookAndFeel.h"
#include "LevelMeter.h"

//==============================================================================
/**
*/

class DelayAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                   private juce::AudioProcessorParameter::Listener
{
public:
    DelayAudioProcessorEditor (DelayAudioProcessor&);
    ~DelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    void parameterValueChanged(int, float) override;
    void parameterGestureChanged(int, bool) override {}
    
    
    DelayAudioProcessor& audioProcessor;
    
    RotaryKnob gainKnob {"Gain", audioProcessor.apvts, gainParamID, true};
    
    RotaryKnob mixKnob {"Mix", audioProcessor.apvts, mixParamID};
    
    RotaryKnob delayTimeKnob {"Time", audioProcessor.apvts, delayTimeParamID};
    
    RotaryKnob feedbackKnob {"Feedback", audioProcessor.apvts, feedbackParamID, true};
    
    RotaryKnob stereowidthKnob {"Stereo widht", audioProcessor.apvts, stereowidthParamID};
    
    RotaryKnob lowCutKnob {"Low Cut", audioProcessor.apvts, lowCutParamID};
    
    RotaryKnob highCutKnob {"High Cut", audioProcessor.apvts, highCutParamID};
    
    RotaryKnob delayNoteKnob {"Delay Note", audioProcessor.apvts, delayNoteParamID};
    
    
    
//    RotaryKnob saturationKnob {"Saturation", audioProcessor.apvts, saturationParamID};
    
    
        
    juce::GroupComponent delayGroup, feedbackGroup, outputGroup;
    
    juce::TextButton tempoSyncButton;
    
    juce::AudioProcessorValueTreeState::ButtonAttachment
    tempoSynchAttachment{
        audioProcessor.apvts, tempoSyncParamID.getParamID(),
        tempoSyncButton
    };
    
    MainLookAndFeel mainLF;
    
    
    void updateDelayKnobs(bool tempoSyncActive);
    
    LevelMeter meter;
    
    juce::ImageButton bypassButton;
    juce::AudioProcessorValueTreeState::ButtonAttachment bypassAttachment {
        audioProcessor.apvts, bypassParamID.getParamID(), bypassButton
    };
    
    juce::GroupComponent logoGroup;
    juce::ImageComponent logoComp;
    
    //juce::AudioProcessorValueTreeState::SliderAttachment attachment {
    //    audioProcessor.apvts, gainParamID.getParamID(), gainKnob.slider                     //1st reference to APVTS from Delayaudioprocessor 2nd parameter ID from Parameters.h
                                                                                            //3rd final argument slider object
        
    //};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayAudioProcessorEditor)
};
