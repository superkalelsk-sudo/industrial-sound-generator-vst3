#include "RingModulator.h"

void RingModulator::prepare(double sampleRate, int samplesPerBlock)
{
    this->sampleRate = static_cast<float>(sampleRate);
    phase = 0.0f;
}

void RingModulator::setFrequency(float freq)
{
    carrierFrequency = freq;
}

void RingModulator::setMix(float m)
{
    mix = juce::jlimit(0.0f, 1.0f, m);
}

float RingModulator::process(float input)
{
    // Generate carrier sine wave
    float carrier = std::sin(juce::MathConstants<float>::twoPi * phase);
    
    // Multiply input by carrier (ring modulation)
    float modulated = input * carrier;
    
    // Wet/dry mix
    float output = modulated * mix + input * (1.0f - mix);
    
    // Update phase for next sample
    phase += carrierFrequency / sampleRate;
    if (phase >= 1.0f)
        phase -= 1.0f;  // Wrap phase
    
    return output;
}
