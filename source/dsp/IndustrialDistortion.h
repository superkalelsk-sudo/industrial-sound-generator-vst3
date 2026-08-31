#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
    IndustrialDistortion - Harmonic Saturation
    
    Soft-clipping distortion using tanh function.
    Creates aggressive, industrial textures.
    Used in Effects Tab.
*/
class IndustrialDistortion
{
public:
    void prepare(double sampleRate, int samplesPerBlock);
    void setDrive(float d);
    void setTone(float freq);
    float process(float input);

private:
    void updateFilterCoeff();
    
    float drive = 0.0f;
    float toneFrequency = 5000.0f;
    float sampleRate = 44100.0f;
    float filterCoeff = 0.1f;
    float filterState = 0.0f;
};
