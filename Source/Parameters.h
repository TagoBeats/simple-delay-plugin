/*
  ==============================================================================

    Parameters.h
    Created: 11 Aug 2025 1:49:23pm
    Author:  Robin

  ==============================================================================
*/

#pragma once
#include  <JuceHeader.h>

const juce::ParameterID gainParamID {"gain", 1};
const juce::ParameterID delayTimeParamID {"delay Time", 1};                     // new const parameter delay Time
const juce::ParameterID mixParamID {"mix", 1};
const juce::ParameterID feedbackParamID {"feedback", 1};
const juce::ParameterID stereowidthParamID {"stereo width", 1};
const juce::ParameterID lowCutParamID {"low cut", 1};
const juce::ParameterID highCutParamID {"high cut", 1};
const juce::ParameterID tempoSyncParamID {"tempoSync", 1};
const juce::ParameterID delayNoteParamID {"delayNote", 1};
const juce::ParameterID bypassParamID {"bypass", 1};
//const juce::ParameterID saturationParamID {"saturation", 1};

class Parameters
{
    public:
        Parameters(juce::AudioProcessorValueTreeState& apvts);          //verwaltet Paramater des Plugins; Gain,Mix,Delay,automations,presets,GUI sync etc.
    
        static juce::AudioProcessorValueTreeState::ParameterLayout
        createParameterLayout();
    
    void update() noexcept;
    void prepareToPlay(double sampleRate) noexcept;
    void reset() noexcept;
    void smoothen() noexcept;
    
    float gain = 0.0f;
    float delayTime = 0.0f;
    float mix = 1.0f;
    float feedback = 0.0f;
    float panL = 0.0f;
    float panR = 1.0f;
    float stereowidth = 0.0f;
    float lowCut = 20.0f;
    float highCut = 20000.0f;
    
    int delayNote = 0;
    bool tempoSync = false;
    //float saturation = 0.0f;
    bool bypassed = false;
    juce::AudioParameterBool* bypassParam;
    
    static constexpr float minDelayTime = 5.0f;         //def const cause refering to a max and min, added to Parameters cause delay time is plugin parameter
    static constexpr float maxDelayTime = 5000.0f;
    
    juce::AudioParameterBool* tempoSyncParam;
        
    
private:
    juce::AudioParameterFloat* gainParam;
    juce::LinearSmoothedValue<float> gainSmoother;
    
    juce::AudioParameterFloat* delayTimeParam;
    
    juce::AudioParameterFloat* mixParam;
    juce::LinearSmoothedValue<float> mixSmoother;
    
    juce::AudioParameterFloat* feedbackParam;
    juce::LinearSmoothedValue<float> feedbackSmoother;
    
    juce::AudioParameterFloat* stereowidthParam;
    juce::LinearSmoothedValue<float> stereowidthSmoother;
    
    juce::AudioParameterFloat* lowCutParam;
    juce::LinearSmoothedValue<float> lowCutSmoother;
    juce::AudioParameterFloat* highCutParam;
    juce::LinearSmoothedValue<float> highCutSmoother;
    
    
    juce::AudioParameterChoice* delayNoteParam;
    
    
    
    //juce::AudioParameterFloat* saturationParam;
    //juce::LinearSmoothedValue<float> saturationSmoother;
    //juce::dsp::WaveShaper<float> saturationShaper;
    
    float targetDelayTime = 0.0f;
    float coeff = 0.0f;                     //one pole smoothing

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Parameters)
};
   

   
