#pragma once

#include <JuceHeader.h>

class PluginProcessor;

//==============================================================================
/**
    EffectsTab - DSP Effect Chain Controls
    
    Sections:
    - Bitcrusher (bit depth reduction)
    - Ring Modulator (metallic modulation)
    - Distortion (harmonic saturation)
    - Multiband EQ (3-band surgical shaping)
    - Reverb (space/ambience)
*/
class EffectsTab : public juce::Component
{
public:
    explicit EffectsTab(PluginProcessor& p);
    ~EffectsTab() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    PluginProcessor& processor;
    
    // UI elements for each effect
    juce::Slider bitcrusherSlider, ringModSlider, distortionSlider;
    juce::Slider eqLowSlider, eqMidSlider, eqHighSlider;
    juce::Slider reverbRoomSlider, reverbDampSlider, reverbWetSlider;
};
