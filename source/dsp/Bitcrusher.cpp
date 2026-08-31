#include "Bitcrusher.h"

void Bitcrusher::prepare(double sampleRate, int samplesPerBlock)
{
    // No state to prepare for bitcrusher
}

void Bitcrusher::setBitDepth(int bits)
{
    bitDepth = juce::jlimit(1, 16, bits);
    scale = static_cast<float>((1 << bitDepth) - 1);
}

float Bitcrusher::process(float input)
{
    // Scale to bit depth range
    float scaled = input * scale;
    
    // Quantize to nearest integer
    float quantized = std::round(scaled);
    
    // Scale back to [-1, 1]
    float output = quantized / scale;
    
    return output;
}
