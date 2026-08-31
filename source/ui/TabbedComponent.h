#pragma once

#include <JuceHeader.h>

class PluginProcessor;
class SamplerTab;
class EffectsTab;
class SynthesisTab;
class MasterTab;

//==============================================================================
/**
    TabbedComponent - Main Tab Manager
    
    Organizes plugin UI into 4 tabs:
    - Sampler (sample loading, drag-drop)
    - Effects (DSP effect chain)
    - Synthesis (pitch, speed, envelope)
    - Master (output, presets)
*/
class TabbedComponent : public juce::Component, public juce::TabbedButtonBar::Listener
{
public:
    explicit TabbedComponent(PluginProcessor& p);
    ~TabbedComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void currentTabChanged(int newCurrentTabIndex, const juce::String& newCurrentTabName) override;

private:
    PluginProcessor& processor;
    
    juce::TabbedButtonBar tabBar;
    std::unique_ptr<SamplerTab> samplerTab;
    std::unique_ptr<EffectsTab> effectsTab;
    std::unique_ptr<SynthesisTab> synthesisTab;
    std::unique_ptr<MasterTab> masterTab;
    
    std::unique_ptr<juce::Component> currentTab;
};
