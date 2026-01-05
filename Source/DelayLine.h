/*
  ==============================================================================

    DelayLine.h
    Created: 20 Aug 2025 6:04:31pm
    Author:  Robin

  ==============================================================================
*/

#pragma once
#include <memory>

class DelayLine
{
    public:
    void setMaximumDelayInSamples(int maxLenghtInSamples);
    void reset() noexcept;
    
    void write(float input) noexcept;
    float read(float delayInSamples) const noexcept;
    
    int getBufferLength() const noexcept
    {
        return bufferLength;
    }
    
    private:
    std::unique_ptr<float[]> buffer;
    int bufferLength = 0;
    int writeIndex= 0;
    
};
