#pragma once

#include <JuceHeader.h>

class PluginProcessor;

//==============================================================================
/**
    SamplerTab - Sample Loading & Management
    
    Features:
    - Drag-and-drop sample loading
    - File browser
    - Waveform preview
    - Sample metadata display
*/
class SamplerTab : public juce::Component, public juce::FileDragAndDropTarget
{
public:
    explicit SamplerTab(PluginProcessor& p);
    ~SamplerTab() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // Drag-drop interface
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

private:
    PluginProcessor& processor;
    
    juce::TextButton browseButton;
    juce::Label dropZoneLabel;
    juce::Label sampleNameLabel;
    juce::Slider volumeSlider;
    
    void openFileBrowser();
    void loadSampleFile(const juce::File& file);
};
