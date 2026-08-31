#include "IndustrialDistortion.h"

void IndustrialDistortion::prepare(double sampleRate, int samplesPerBlock)
{
    this->sampleRate = static_cast<float>(sampleRate);
    filterState = 0.0f;
    updateFilterCoeff();
}

void IndustrialDistortion::setDrive(float d)
{
    drive = juce::jlimit(0.0f, 10.0f, d);
}

void IndustrialDistortion::setTone(float freq)
{
    toneFrequency = juce::jlimit(100.0f, 20000.0f, freq);
    updateFilterCoeff();
}

void IndustrialDistortion::updateFilterCoeff()
{
    // Simple RC filter coefficient
    float rc = 1.0f / (juce::MathConstants<float>::twoPi * toneFrequency);
    float dt = 1.0f / sampleRate;
    filterCoeff = dt / (rc + dt);
}

float IndustrialDistortion::process(float input)
{
    // Step 1: Apply drive (gain)
    float driven = input * (1.0f + drive);
    
    // Step 2: Soft clip using tanh (smooth saturation)
    float clipped = std::tanh(driven);
    
    // Step 3: One-pole low-pass filter for tone control
    filterState += filterCoeff * (clipped - filterState);
    
    return filterState;
}
