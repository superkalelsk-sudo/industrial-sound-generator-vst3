#pragma once

#include <JuceHeader.h>

class PluginProcessor;

//==============================================================================
/**
    SynthesisTab - Sample Playback Parameters
    
    Controls:
    - Pitch transposition (-24 to +24 semitones)
    - Playback speed (0.5x to 2.0x)
    - Attack/Release envelope
    - Velocity sensitivity
*/
class SynthesisTab : public juce::Component
{
public:
    explicit SynthesisTab(PluginProcessor& p);
    ~SynthesisTab() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    PluginProcessor& processor;
    
    juce::Slider pitchSlider, speedSlider;
    juce::Slider attackSlider, releaseSlider;
};
