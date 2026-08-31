#pragma once

#include <JuceHeader.h>

class PluginProcessor;

//==============================================================================
/**
    PresetManager - Plugin State Save/Load
    
    Handles:
    - Saving presets as JSON/XML
    - Loading saved presets
    - Preset browser
    - Default factory presets
*/
class PresetManager
{
public:
    explicit PresetManager(PluginProcessor& p);
    
    void savePreset(const juce::String& presetName);
    void loadPreset(const juce::String& presetName);
    juce::StringArray getPresetNames() const;

private:
    PluginProcessor& processor;
    juce::File presetDirectory;
};
