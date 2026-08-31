#include "SampleManager.h"

void SampleManager::prepare(double sampleRate)
{
    this->sampleRate = sampleRate;
    readPosition = 0;
    isPlaying = false;
}

void SampleManager::loadSampleFile(const juce::File& file)
{
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
    if (reader != nullptr)
    {
        sampleBuffer.setSize(static_cast<int>(reader->numChannels), 
                           static_cast<int>(reader->lengthInSamples));
        reader->read(&sampleBuffer, 0, static_cast<int>(reader->lengthInSamples), 0, true, true);
        readPosition = 0;
    }
}

void SampleManager::triggerNote(int midiNote, int velocity)
{
    // Simple trigger: start reading from beginning
    readPosition = 0;
    isPlaying = true;
    currentPitch = 1.0f;  // Would be calculated from midiNote for transposition
}

void SampleManager::releaseNote(int midiNote)
{
    // Stop playback
    isPlaying = false;
}

float SampleManager::getNextSample()
{
    if (!isPlaying || sampleBuffer.getNumSamples() == 0)
        return 0.0f;
    
    // Simple playback (no interpolation)
    float sample = sampleBuffer.getSample(0, readPosition);
    readPosition++;
    
    // Loop or stop
    if (readPosition >= sampleBuffer.getNumSamples())
    {
        isPlaying = false;
        readPosition = 0;
    }
    
    return sample;
}
