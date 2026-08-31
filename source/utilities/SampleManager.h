#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
    SampleManager - One-Shot Sample Loading & Playback
    
    Handles:
    - Loading WAV/AIFF/MP3/FLAC files
    - MIDI note triggering
    - Pitch-shifted playback
    - Envelope application
*/
class SampleManager
{
public:
    void prepare(double sampleRate);
    void loadSampleFile(const juce::File& file);
    void triggerNote(int midiNote, int velocity);
    void releaseNote(int midiNote);
    float getNextSample();

private:
    juce::AudioBuffer<float> sampleBuffer;
    double sampleRate = 44100.0;
    int readPosition = 0;
    bool isPlaying = false;
    float currentPitch = 1.0f;
};
