#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ui/TabbedComponent.h"

//==============================================================================
/**
    PluginEditor - Main GUI
    
    Features tabbed interface for organized access to:
    - Sampler Tab (sample loading, drag-drop)
    - Effects Tab (DSP effect chain controls)
    - Synthesis Tab (pitch, speed, envelope)
    - Master Tab (output levels, presets)
*/
class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor(PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    PluginProcessor& processorRef;
    
    // Tabbed interface
    std::unique_ptr<TabbedComponent> tabbedComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
