#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
    ReverbEngine - Freeverb Reverberator
    
    Schroeder reverberator with comb and allpass filters.
    Adds space and ambience.
    Used in Effects Tab.
*/
class ReverbEngine
{
public:
    void prepare(double sampleRate, int samplesPerBlock);
    void setRoomSize(float size);
    void setDampening(float damp);
    void setWetLevel(float wet);
    void setDryLevel(float dry);
    float process(float input);

private:
    struct CombFilter
    {
        float buffer[8192] = {};
        int bufferIndex = 0;
        float feedback = 0.84f;
        float damp1 = 0.5f, damp2 = 0.5f;
        float filterStore = 0.0f;
    };
    
    struct AllpassFilter
    {
        float buffer[4096] = {};
        int bufferIndex = 0;
        float feedback = 0.5f;
    };
    
    CombFilter combs[8];
    AllpassFilter allpasses[4];
    float wetLevel = 0.3f;
    float dryLevel = 0.7f;
    float sampleRate = 44100.0f;
};
