#include "ReverbEngine.h"

void ReverbEngine::prepare(double sampleRate, int samplesPerBlock)
{
    this->sampleRate = static_cast<float>(sampleRate);
    
    for (int i = 0; i < 8; ++i)
    {
        combs[i].bufferIndex = 0;
        combs[i].feedback = 0.84f;
        combs[i].damp1 = 0.5f;
        combs[i].damp2 = 1.0f - 0.5f;
        combs[i].filterStore = 0.0f;
        std::fill(combs[i].buffer, combs[i].buffer + 8192, 0.0f);
    }
    
    for (int i = 0; i < 4; ++i)
    {
        allpasses[i].bufferIndex = 0;
        allpasses[i].feedback = 0.5f;
        std::fill(allpasses[i].buffer, allpasses[i].buffer + 4096, 0.0f);
    }
}

void ReverbEngine::setRoomSize(float size)
{
    float fb = size * 0.28f + 0.7f;
    for (int i = 0; i < 8; ++i)
        combs[i].feedback = fb;
}

void ReverbEngine::setDampening(float damp)
{
    float d = damp * 0.4f;
    for (int i = 0; i < 8; ++i)
    {
        combs[i].damp1 = d;
        combs[i].damp2 = 1.0f - d;
    }
}

void ReverbEngine::setWetLevel(float wet)
{
    wetLevel = wet * 3.0f;
}

void ReverbEngine::setDryLevel(float dry)
{
    dryLevel = dry * 2.0f;
}

float ReverbEngine::process(float input)
{
    float output = 0.0f;
    
    // Comb filters (parallel)
    for (int i = 0; i < 8; ++i)
    {
        auto& comb = combs[i];
        float bufOut = comb.buffer[comb.bufferIndex];
        
        comb.filterStore = (bufOut * (1.0f - comb.damp1)) + (comb.filterStore * comb.damp1);
        comb.buffer[comb.bufferIndex] = input + (comb.filterStore * comb.feedback);
        comb.bufferIndex = (comb.bufferIndex + 1) % 8192;
        
        output += bufOut;
    }
    
    // Allpass filters (series, applied to comb output)
    for (int i = 0; i < 4; ++i)
    {
        auto& allpass = allpasses[i];
        float bufOut = allpass.buffer[allpass.bufferIndex];
        
        float toWrite = output + (bufOut * allpass.feedback);
        allpass.buffer[allpass.bufferIndex] = toWrite;
        allpass.bufferIndex = (allpass.bufferIndex + 1) % 4096;
        
        output = bufOut - (toWrite * allpass.feedback);
    }
    
    // Wet/dry mix
    return output * wetLevel + input * dryLevel;
}
