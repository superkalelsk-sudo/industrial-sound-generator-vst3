#include "MasterTab.h"
#include "../PluginProcessor.h"

//==============================================================================
MasterTab::MasterTab(PluginProcessor& p)
    : processor(p)
{
    // Master gain slider
    masterGainSlider.setRange(-12.0, 12.0, 0.1);
    masterGainSlider.setValue(0.0);
    masterGainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    masterGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(masterGainSlider);

    // Save preset button
    savePresetButton.setButtonText("💾 Save Preset");
    savePresetButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2196F3));
    savePresetButton.onClick = [this]
    {
        juce::AlertWindow::showMessageBoxAsync(
            juce::AlertWindow::InfoIcon,
            "Save Preset",
            "Enter preset name:",
            "Save", nullptr);
    };
    addAndMakeVisible(savePresetButton);

    // Load preset button
    loadPresetButton.setButtonText("📂 Load Preset");
    loadPresetButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff4CAF50));
    addAndMakeVisible(loadPresetButton);

    // Preset name label
    presetNameLabel.setText("No preset loaded", juce::dontSendNotification);
    presetNameLabel.setColour(juce::Label::textColourId, juce::Colour(0xffCCCCCC));
    addAndMakeVisible(presetNameLabel);

    // CPU meter label
    cpuMeterLabel.setText("CPU: 0%", juce::dontSendNotification);
    cpuMeterLabel.setColour(juce::Label::textColourId, juce::Colour(0xff4CAF50));
    addAndMakeVisible(cpuMeterLabel);
}

MasterTab::~MasterTab()
{
}

//==============================================================================
void MasterTab::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1a1a1a));
    
    g.setColour(juce::Colour(0xffCCCCCC));
    g.setFont(juce::Font(18.0f, juce::Font::bold));
    g.drawText("Master Output", 50, 30, 250, 30, juce::Justification::centred);
    
    g.setFont(juce::Font(14.0f));
    g.drawText("Presets", 50, 240, 250, 30, juce::Justification::left);
    
    g.setFont(juce::Font(12.0f));
    g.setColour(juce::Colour(0xff888888));
    g.drawText("Current:", 50, 280, 150, 20, juce::Justification::left);
}

void MasterTab::resized()
{
    auto bounds = getLocalBounds().reduced(20);
    
    // Master gain knob (top center)
    masterGainSlider.setBounds(bounds.removeFromTop(250).withTrimmedLeft(150).withTrimmedRight(150));
    bounds.removeFromTop(30);
    
    // Preset section
    bounds.removeFromTop(50);
    auto presetBounds = bounds.removeFromTop(100);
    
    presetNameLabel.setBounds(200, 285, 350, 20);
    
    auto buttonRow = presetBounds.removeFromTop(40);
    savePresetButton.setBounds(buttonRow.removeFromLeft(200));
    buttonRow.removeFromLeft(20);
    loadPresetButton.setBounds(buttonRow.removeFromLeft(200));
    
    // CPU meter (bottom right)
    cpuMeterLabel.setBounds(getWidth() - 180, getHeight() - 60, 160, 40);
}
