#include "SynthesisTab.h"
#include "../PluginProcessor.h"

//==============================================================================
SynthesisTab::SynthesisTab(PluginProcessor& p)
    : processor(p)
{
    // Pitch slider
    pitchSlider.setRange(-24.0, 24.0, 1.0);
    pitchSlider.setValue(0.0);
    pitchSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    pitchSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(pitchSlider);

    // Speed slider
    speedSlider.setRange(0.5, 2.0, 0.01);
    speedSlider.setValue(1.0);
    speedSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(speedSlider);

    // Attack slider
    attackSlider.setRange(0.0, 100.0, 1.0);
    attackSlider.setValue(10.0);
    attackSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    addAndMakeVisible(attackSlider);

    // Release slider
    releaseSlider.setRange(10.0, 1000.0, 10.0);
    releaseSlider.setValue(100.0);
    releaseSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    addAndMakeVisible(releaseSlider);
}

SynthesisTab::~SynthesisTab()
{
}

//==============================================================================
void SynthesisTab::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1a1a1a));
    
    // Draw labels
    g.setColour(juce::Colour(0xffCCCCCC));
    g.setFont(juce::Font(18.0f, juce::Font::bold));
    
    g.drawText("Pitch", 50, 30, 150, 30, juce::Justification::centred);
    g.drawText("Speed", 300, 30, 150, 30, juce::Justification::centred);
    
    g.setFont(juce::Font(14.0f));
    g.drawText("Attack (ms)", 80, 250, 150, 20, juce::Justification::left);
    g.drawText("Release (ms)", 80, 310, 150, 20, juce::Justification::left);
    
    // Draw info text
    g.setFont(juce::Font(11.0f));
    g.setColour(juce::Colour(0xff888888));
    g.drawText("Use pitch to transpose samples ±24 semitones", 80, 360, 400, 30, juce::Justification::left);
    g.drawText("Adjust speed for time-stretching (0.5x - 2.0x)", 80, 400, 400, 30, juce::Justification::left);
}

void SynthesisTab::resized()
{
    auto bounds = getLocalBounds().reduced(20);
    
    // Rotary knobs (top)
    pitchSlider.setBounds(30, 60, 180, 180);
    speedSlider.setBounds(280, 60, 180, 180);
    
    // Envelope sliders (bottom)
    auto sliderBounds = bounds.removeFromBottom(200).reduced(20);
    
    auto attackBounds = sliderBounds.removeFromTop(50);
    attackBounds.removeFromLeft(150);
    attackSlider.setBounds(attackBounds);
    
    sliderBounds.removeFromTop(20);
    
    auto releaseBounds = sliderBounds.removeFromTop(50);
    releaseBounds.removeFromLeft(150);
    releaseSlider.setBounds(releaseBounds);
}
