#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
    Bitcrusher - Digital Grit Effect
    
    Reduces bit depth to create lo-fi, digital textures.
    Used in Effects Tab.
*/
class Bitcrusher
{
public:
    void prepare(double sampleRate, int samplesPerBlock);
    void setBitDepth(int bits);
    float process(float input);

private:
    int bitDepth = 16;
    float scale = (1 << 16) - 1;
};
