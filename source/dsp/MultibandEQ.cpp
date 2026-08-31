#include "MultibandEQ.h"

void MultibandEQ::prepare(double sampleRate, int samplesPerBlock)
{
    this->sampleRate = static_cast<float>(sampleRate);
    
    // Initialize band centers
    bands[0].frequency = 125.0f;   // Low
    bands[1].frequency = 1000.0f;  // Mid
    bands[2].frequency = 8000.0f;  // High
    
    // All Q = 1.0 (gentle slopes)
    for (int i = 0; i < 3; ++i)
        bands[i].q = 1.0f;
    
    updateCoefficients();
}

void MultibandEQ::setGain(int band, float gainDb)
{
    if (band >= 0 && band < 3)
    {
        bands[band].gain = juce::jlimit(-12.0f, 12.0f, gainDb);
        updateCoefficients();
    }
}

void MultibandEQ::updateCoefficients()
{
    for (int i = 0; i < 3; ++i)
    {
        Band& band = bands[i];
        float A = std::pow(10.0f, band.gain / 40.0f);
        float w0 = juce::MathConstants<float>::twoPi * band.frequency / sampleRate;
        float alpha = std::sin(w0) / (2.0f * band.q);
        
        // Peaking EQ biquad coefficients
        band.b0 = 1.0f + alpha * A;
        band.b1 = -2.0f * std::cos(w0);
        band.b2 = 1.0f - alpha * A;
        band.a1 = -2.0f * std::cos(w0);
        band.a2 = 1.0f - alpha;
        
        // Normalize by a0
        float a0 = 1.0f + alpha / A;
        band.b0 /= a0;
        band.b1 /= a0;
        band.b2 /= a0;
        band.a1 /= a0;
        band.a2 /= a0;
    }
}

float MultibandEQ::process(float input)
{
    float output = 0.0f;
    
    for (int i = 0; i < 3; ++i)
    {
        Band& band = bands[i];
        
        // Biquad difference equation
        float y = band.b0 * input + band.b1 * band.z1 + band.b2 * band.z2
                - band.a1 * band.z1 - band.a2 * band.z2;
        
        // Update state
        band.z2 = band.z1;
        band.z1 = y;
        
        output += y;
    }
    
    return output;
}
