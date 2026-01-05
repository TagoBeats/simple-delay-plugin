/*
  ==============================================================================

    LevelMeter.h
    Created: 21 Aug 2025 3:28:46pm
    Author:  Robin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Measurement.h"

//==============================================================================
/*
*/
class LevelMeter  : public juce::Component, private juce::Timer             //adding juce::Timer so LevelMeter class inherits from it
{
public:
    LevelMeter(Measurement& measurementL, Measurement& measurementR);
    ~LevelMeter() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;          //reads the peak levels and redraw components
    
    Measurement& measurementL;
    Measurement& measurementR;
    
    static constexpr float maxdB = 6.0f;
    static constexpr float mindB = -60.0f;
    static constexpr float stepdB = 6.0f;
    
    float maxPos = 0.0f;
    float minPos = 0.0f;
    
    int positionForLevel(float dBLevel) const noexcept
    {
        return int(std::round(juce::jmap(dBLevel, maxdB, mindB, maxPos, minPos)));
    }
    
    static constexpr float clampdB = -120.0f;
    static constexpr float clampLevel = 0.000001f; //-120dB
    
    float dbLevelL;
    float dbLevelR;
    
    void drawLevel(juce::Graphics& g, float level, int x, int width);
    
    static constexpr int refreshRate = 60;
    float decay = 0.0f;
    float levelL = clampLevel;
    float levelR = clampLevel;
    
    void updateLevel(float newLevel, float& smoothedLevel, float& leveldB)
    const;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
