#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
    MultibandEQ - 3-Band Parametric EQ
    
    Surgical frequency shaping across 3 bands.
    Used in Effects Tab.
*/
class MultibandEQ
{
public:
    void prepare(double sampleRate, int samplesPerBlock);
    void setGain(int band, float gainDb);
    float process(float input);

private:
    struct Band
    {
        float gain = 0.0f;
        float frequency = 1000.0f;
        float q = 1.0f;
        float b0, b1, b2, a1, a2;
        float z1 = 0.0f, z2 = 0.0f;
    };
    
    Band bands[3];
    float sampleRate = 44100.0f;
    
    void updateCoefficients();
};
