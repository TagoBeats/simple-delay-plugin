/*
  ==============================================================================

    DelayLine.cpp
    Created: 20 Aug 2025 6:04:31pm
    Author:  Robin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DelayLine.h"

void DelayLine::setMaximumDelayInSamples(int maxLengthinSamples)
{
    jassert(maxLengthinSamples > 0);        //checks if requestet delayline is >0, good for preventing bugs etc.
    
    int paddedLength = maxLengthinSamples + 2;      //ammount of memory for maxLengthinSamples + 1 (because 5 samples got maximum delay of 4)
    
    if (bufferLength < paddedLength) {      //avoids allocation memory if existing buffer ist kong enough
        bufferLength = paddedLength;
        
        buffer.reset(new float[size_t(bufferLength)]);      //allocates new memory
    }
}

void DelayLine::reset() noexcept
{
    writeIndex = bufferLength - 1;      //writing allways starts at index 0 now
    
    for (size_t i = 0; i < size_t(bufferLength); ++i) {         // index i starts at 0 and increases until i < bufferlenght - 1 is no longer true, i go from 0 to buffer -1
        buffer[i] = 0.0f;
    }
}

void DelayLine::write(float input) noexcept
{
    jassert(bufferLength > 0);
    
    writeIndex += 1;        //increment writeindex by 1
    
    if (writeIndex >= bufferLength) {  //checks if its at the last element in memory buffer, if so its going back to zero
        writeIndex = 0;
    }
    buffer[size_t(writeIndex)] = input;  //put the new sample into memory buffer, it would work without size_t but its recommended by JUCE
}

//float DelayLine::read(float delayInSamples) const noexcept      //const because it only reads and doesnt change
//{
//    jassert(delayInSamples >= 0.0f);
//    jassert(delayInSamples <= bufferLength - 1.0f);
    
//    int integerDelay = int(delayInSamples);       //calculate read index, requested delaylength - writeIndexLength
    
//    int readIndexA = writeIndex - integerDelay;
//    if (readIndexA < 0) {            //checks where it is and if it need to wrap arround
//        readIndexA += bufferLength;
//    }
    
//    int readIndexB = readIndexA -1;
//    if (readIndexB < 0) {
//        readIndexB += bufferLength;
//    }
    
//    float sampleA = buffer[size_t(readIndexA)];
//    float sampleB = buffer[size_t(readIndexB)];
    
//    float fraction = delayInSamples - float(integerDelay);
//    return sampleA + fraction * (sampleB - sampleA);
    
//}         linear interpolation

float DelayLine::read(float delayInSamples) const noexcept
{
    jassert(delayInSamples >= 1.0f);
    jassert(delayInSamples <= bufferLength - 2.0f);
    
    int integerDelay = int(delayInSamples);

    int readIndexA = writeIndex - integerDelay + 1;
    int readIndexB = readIndexA - 1;
    int readIndexC = readIndexA - 2;
    int readIndexD = readIndexA - 3;

    if (readIndexD < 0) {
        readIndexD += bufferLength;
        if (readIndexC < 0) {
            readIndexC += bufferLength;
            if (readIndexB < 0) {
                readIndexB += bufferLength;
                if (readIndexA < 0) {
                    readIndexA += bufferLength;
                }
            }
        }
    }

    float sampleA = buffer[size_t(readIndexA)];
    float sampleB = buffer[size_t(readIndexB)];
    float sampleC = buffer[size_t(readIndexC)];
    float sampleD = buffer[size_t(readIndexD)];

    float fraction = delayInSamples - float(integerDelay);
    float slope0 = (sampleC - sampleA) * 0.5f;
    float slope1 = (sampleD - sampleB) * 0.5f;
    float v = sampleB - sampleC;
    float w = slope0 + v;
    float a = w + v + slope1;
    float b = w + a;
    float stage1 = a * fraction - b;
    float stage2 = stage1 * fraction + slope0;
    return stage2 * fraction + sampleB;
    }

  
