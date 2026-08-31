#pragma once

#include <JuceHeader.h>

class PluginProcessor;

//==============================================================================
/**
    MasterTab - Output & Preset Management
    
    Features:
    - Output level control
    - VU meter display
    - CPU monitoring
    - Preset browser
    - Preset save/load
*/
class MasterTab : public juce::Component
{
public:
    explicit MasterTab(PluginProcessor& p);
    ~MasterTab() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    PluginProcessor& processor;
    
    juce::Slider masterGainSlider;
    juce::TextButton savePresetButton, loadPresetButton;
    juce::Label presetNameLabel;
    juce::Label cpuMeterLabel;
};
