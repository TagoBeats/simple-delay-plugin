/*
  ==============================================================================

    Parameters.cpp
    Created: 11 Aug 2025 1:49:23pm
    Author:  Robin

  ==============================================================================
*/

#include "Parameters.h"
#include "DSP.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts,
                          const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination);
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    castParameter(apvts, gainParamID, gainParam);
    castParameter(apvts, delayTimeParamID, delayTimeParam);                         // adding pointer to parameter from Parameter Header delayTime
    castParameter(apvts, mixParamID, mixParam);
    castParameter(apvts, feedbackParamID, feedbackParam);
    castParameter(apvts, stereowidthParamID, stereowidthParam);
    castParameter(apvts, lowCutParamID, lowCutParam);
    castParameter(apvts, highCutParamID, highCutParam);
    castParameter(apvts, tempoSyncParamID, tempoSyncParam);
    castParameter(apvts, delayNoteParamID, delayNoteParam);
//    castParameter(apvts, saturationParamID, saturationParam);
    castParameter(apvts, bypassParamID, bypassParam);
}

static juce::String stringFromMilliseconds(float value, int)
{
    if (value < 10.0f) {
        return juce::String(value, 2) + "ms";
    } else if (value < 100.0f) {
        return juce::String(value, 1) + "ms" ;
    } else if (value < 1000.f) {
        return juce::String(int(value)) + "ms" ;
    } else {
        return juce::String(value * 0.001f, 2) + "s" ;
    }
}

static float millisecondsFromString(const juce::String& text)
{
    float value = text.getFloatValue();
    if (!text.endsWithIgnoreCase("ms")) {
        if (text.endsWithIgnoreCase("s") || value < Parameters::minDelayTime) {
            return value * 1000.0f;
        }
    }
    return  value;
}

static juce::String stringFromDecibels(float value, int)
{
    return juce::String(value, 1) + "dB";
}

static juce::String stringFromPercent(float value, int)
{
    return juce::String(int(value)) + "%";
}

static juce::String stringFromHz(float value, int)
{
    if (value < 1000.0f) {
        return juce::String(int(value)) + "Hz";
    } else if (value > 1000.0f){
        return juce::String(value / 1000.0f, 2) + "k";
    } else {
        return juce::String(value / 1000.0f, 1) + "k";
    }
}

static float hzFromString(const juce::String& str)
{
    float value = str.getFloatValue();          //any value under 20 will be interpreted as kHz
    if (value < 20.0f) {
        return value * 1000.0f;
    }
    return value;
}

juce::AudioProcessorValueTreeState::ParameterLayout
    Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                gainParamID,
                "Output Gain",
                juce::NormalisableRange<float>
                { -12.0f, 12.0f}, 0.0f,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromDecibels)
        ));
                
                                                                          
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                delayTimeParamID,
                "Delay Time",
                juce::NormalisableRange<float> {minDelayTime, maxDelayTime, 0.001f, 0.20f },      // set range from minDelay to maxDelay Time, 0.001 is step size and 0.25 skew factor
                100.0f,                                                                           // default setting 100.0f or 100ms
                juce::AudioParameterFloatAttributes()
                        .withStringFromValueFunction(stringFromMilliseconds)                                // tells how to use, string from value
                        .withValueFromStringFunction(millisecondsFromString)
        ));
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                mixParamID,
                "Mix",
                juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
                100.0f,
                juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)        //display value in percantages
        ));
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                feedbackParamID,
                "Feedback",
                juce::NormalisableRange<float>(-100.0f, 100.0f, 1.0f),
                20.0f,  // make sure this is zero!
                juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)
        ));
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                stereowidthParamID,
                "Stereo width",
                juce::NormalisableRange<float>(-100.0f, 100.0f, 1.0f),
                0.0f,
                juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)
        ));
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            lowCutParamID,
            "Low Cut",
            juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f),
            20.0f,
            juce::AudioParameterFloatAttributes()
                .withStringFromValueFunction(stringFromHz)
                .withValueFromStringFunction(hzFromString)
        ));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            highCutParamID,
            "High Cut",
            juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f),
            20000.0f,
            juce::AudioParameterFloatAttributes()
                .withStringFromValueFunction(stringFromHz)
                .withValueFromStringFunction(hzFromString)
        ));
        
        layout.add(std::make_unique<juce::AudioParameterBool>(tempoSyncParamID, "TempoSync", false));
        juce::StringArray noteLengths = {
            "1/32",
            "1/16 trip",
            "1/32 dot",
            "1/16",
            "1/8 trip",
            "1/16 dot",
            "1/8",
            "1/4 trip",
            "1/8 dot",
            "1/4",
            "1/2 trip",
            "1/4 dot",
            "1/2",
            "1/1 trip",
            "1/2 dot",
            "1/1",
        };

        layout.add(std::make_unique<juce::AudioParameterChoice>(
            delayNoteParamID, "Delay Note", noteLengths, 9));
        
        layout.add(std::make_unique<juce::AudioParameterBool>(
            bypassParamID, "Bypass", false));

        
//        layout.add(std::make_unique<juce::AudioParameterFloat>(
//                saturationParamID,
//                "Saturation",
//                juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
//                0.0f,
//                juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)
//        ));
                                                        
                
    return layout;
}
void Parameters::update() noexcept
{
    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));   //erst holen wir den gainParm, dann converten wir von dB in lin, dann smooth
        
    targetDelayTime = delayTimeParam->get();
    if (delayTime == 0.0f) {
        delayTime = targetDelayTime;
    }
    
    mixSmoother.setTargetValue(mixParam->get() * 0.01f);
    
    feedbackSmoother.setTargetValue(feedbackParam->get() * 0.01f);
    
    stereowidthSmoother.setTargetValue(stereowidthParam->get() * 0.01f);

    lowCutSmoother.setTargetValue(lowCutParam->get());
    highCutSmoother.setTargetValue(highCutParam->get());
    
    delayNote = delayNoteParam->getIndex();
    tempoSync = tempoSyncParam->get();
    //bypassed = bypassParam->get();
    //saturationSmoother.setTargetValue(saturationParam->get());

   
}

    
void Parameters::reset() noexcept
{
    gain = 0.0f;
    gainSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    
    delayTime = 0.0f;
    
    mix = 1.0f;
    mixSmoother.setCurrentAndTargetValue(mixParam->get() * 0.01f);                      //mixParam gets 0.01 what is equivalent to dividing values by 100
    
    feedback = 0.0f;
    feedbackSmoother.setCurrentAndTargetValue(feedbackParam->get() * 0.01f);
    
    panL = 0.0f;
    panR = 1.0f;
    
    stereowidth = 0.0f;
    stereowidthSmoother.setCurrentAndTargetValue(stereowidthParam->get() * 0.01f);
    
    lowCut = 20.0f;
    lowCutSmoother.setCurrentAndTargetValue(lowCutParam->get());
    
    highCut = 20000.0f;
    highCutSmoother.setCurrentAndTargetValue(highCutParam->get());
    
//    saturation = 0.0f;
//    saturationSmoother.setCurrentAndTargetValue(saturationParam->get());
    
}

void Parameters::smoothen() noexcept
{
    gain = gainSmoother.getNextValue();
    
    delayTime = targetDelayTime;
    
    mix = mixSmoother.getNextValue();
    
    feedback = feedbackSmoother.getNextValue();
    
    //stereowidth = stereowidthSmoother.getNextValue();
    
    panningEqualPower(stereowidthSmoother.getNextValue(), panL, panR);
    
    lowCut = lowCutSmoother.getNextValue();
    highCut = highCutSmoother.getNextValue();
    
//    saturation = saturationSmoother.getNextValue();

}

void Parameters::prepareToPlay(double sampleRate) noexcept
{
    double duration = 0.02;
    gainSmoother.reset(sampleRate, duration);
    
    //gainSmoother.reset(sampleRate, 0.05);
    
    coeff = 1.0f - std::exp(-1.0f / (0.2f * float(sampleRate)));
    
    mixSmoother.reset(sampleRate, duration);
    
    feedbackSmoother.reset(sampleRate, duration);
    
    stereowidthSmoother.reset(sampleRate, duration);
    
    lowCutSmoother.reset(sampleRate, duration);
    
    highCutSmoother.reset(sampleRate, duration);
    
//    saturationSmoother.reset(sampleRate, duration);
}
