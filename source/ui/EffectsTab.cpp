#include "EffectsTab.h"
#include "../PluginProcessor.h"

//==============================================================================
EffectsTab::EffectsTab(PluginProcessor& p)
    : processor(p)
{
    // Bitcrusher section
    bitcrusherSlider.setRange(0.0, 1.0, 0.01);
    bitcrusherSlider.setValue(0.0);
    bitcrusherSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    bitcrusherSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(bitcrusherSlider);

    // Ring Modulator section
    ringModSlider.setRange(0.0, 1.0, 0.01);
    ringModSlider.setValue(0.0);
    ringModSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    ringModSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(ringModSlider);

    // Distortion section
    distortionSlider.setRange(0.0, 1.0, 0.01);
    distortionSlider.setValue(0.0);
    distortionSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    distortionSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(distortionSlider);

    // EQ sliders
    eqLowSlider.setRange(-12.0, 12.0, 0.1);
    eqLowSlider.setValue(0.0);
    eqLowSlider.setSliderStyle(juce::Slider::LinearVertical);
    addAndMakeVisible(eqLowSlider);

    eqMidSlider.setRange(-12.0, 12.0, 0.1);
    eqMidSlider.setValue(0.0);
    eqMidSlider.setSliderStyle(juce::Slider::LinearVertical);
    addAndMakeVisible(eqMidSlider);

    eqHighSlider.setRange(-12.0, 12.0, 0.1);
    eqHighSlider.setValue(0.0);
    eqHighSlider.setSliderStyle(juce::Slider::LinearVertical);
    addAndMakeVisible(eqHighSlider);

    // Reverb sliders
    reverbRoomSlider.setRange(0.0, 1.0, 0.01);
    reverbRoomSlider.setValue(0.5);
    reverbRoomSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    addAndMakeVisible(reverbRoomSlider);

    reverbDampSlider.setRange(0.0, 1.0, 0.01);
    reverbDampSlider.setValue(0.5);
    reverbDampSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    addAndMakeVisible(reverbDampSlider);

    reverbWetSlider.setRange(0.0, 1.0, 0.01);
    reverbWetSlider.setValue(0.3);
    reverbWetSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    addAndMakeVisible(reverbWetSlider);
}

EffectsTab::~EffectsTab()
{
}

//==============================================================================
void EffectsTab::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1a1a1a));
    
    // Draw section headers
    g.setColour(juce::Colour(0xffCCCCCC));
    g.setFont(juce::Font(18.0f, juce::Font::bold));
    
    g.drawText("Bitcrusher", 30, 20, 200, 20, juce::Justification::left);
    g.drawText("Ring Modulator", 250, 20, 200, 20, juce::Justification::left);
    g.drawText("Distortion", 470, 20, 200, 20, juce::Justification::left);
    
    g.drawText("Multiband EQ", 30, 200, 200, 20, juce::Justification::left);
    g.setFont(juce::Font(12.0f));
    g.drawText("Low", 30, 235, 80, 15, juce::Justification::centred);
    g.drawText("Mid", 130, 235, 80, 15, juce::Justification::centred);
    g.drawText("High", 230, 235, 80, 15, juce::Justification::centred);
    
    g.setFont(juce::Font(14.0f, juce::Font::bold));
    g.drawText("Reverb", 30, 340, 200, 20, juce::Justification::left);
    g.setFont(juce::Font(11.0f));
    g.drawText("Room Size", 30, 370, 150, 15, juce::Justification::left);
    g.drawText("Dampening", 30, 410, 150, 15, juce::Justification::left);
    g.drawText("Wet Level", 30, 450, 150, 15, juce::Justification::left);
}

void EffectsTab::resized()
{
    auto bounds = getLocalBounds().reduced(20);
    
    // FX sliders (top row - rotary)
    bitcrusherSlider.setBounds(30, 50, 150, 150);
    ringModSlider.setBounds(250, 50, 150, 150);
    distortionSlider.setBounds(470, 50, 150, 150);
    
    // EQ sliders (vertical)
    eqLowSlider.setBounds(30, 260, 80, 70);
    eqMidSlider.setBounds(130, 260, 80, 70);
    eqHighSlider.setBounds(230, 260, 80, 70);
    
    // Reverb sliders (horizontal)
    reverbRoomSlider.setBounds(200, 370, 300, 30);
    reverbDampSlider.setBounds(200, 410, 300, 30);
    reverbWetSlider.setBounds(200, 450, 300, 30);
}
