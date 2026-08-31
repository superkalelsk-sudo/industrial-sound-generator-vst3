#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
    RingModulator - Metallic Modulation
    
    Multiplies audio by sinusoidal carrier for bell-like tones.
    Used in Effects Tab.
*/
class RingModulator
{
public:
    void prepare(double sampleRate, int samplesPerBlock);
    void setFrequency(float freq);
    void setMix(float m);
    float process(float input);

private:
    float carrierFrequency = 100.0f;
    float phase = 0.0f;
    float sampleRate = 44100.0f;
    float mix = 0.0f;
};
