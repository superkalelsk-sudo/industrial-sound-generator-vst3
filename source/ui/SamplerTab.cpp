#include "SamplerTab.h"
#include "../PluginProcessor.h"

//==============================================================================
SamplerTab::SamplerTab(PluginProcessor& p)
    : processor(p)
{
    // Browse button
    browseButton.setButtonText("📁 Browse Sample...");
    browseButton.onClick = [this] { openFileBrowser(); };
    browseButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff4CAF50));
    addAndMakeVisible(browseButton);

    // Drop zone label
    dropZoneLabel.setText("🎵 Drag & Drop Sample Here", juce::dontSendNotification);
    dropZoneLabel.setJustificationType(juce::Justification::centred);
    dropZoneLabel.setFont(juce::Font(24.0f));
    dropZoneLabel.setColour(juce::Label::textColourId, juce::Colour(0xff888888));
    addAndMakeVisible(dropZoneLabel);

    // Sample name label
    sampleNameLabel.setText("No sample loaded", juce::dontSendNotification);
    sampleNameLabel.setColour(juce::Label::textColourId, juce::Colour(0xffCCCCCC));
    addAndMakeVisible(sampleNameLabel);

    // Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(1.0);
    volumeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    addAndMakeVisible(volumeSlider);
}

SamplerTab::~SamplerTab()
{
}

//==============================================================================
void SamplerTab::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1a1a1a));
    
    // Draw drop zone border
    g.setColour(juce::Colour(0xff4CAF50).withAlpha(0.3f));
    g.drawRect(100, 100, getWidth() - 200, getHeight() - 200, 2);
}

void SamplerTab::resized()
{
    auto bounds = getLocalBounds().reduced(20);
    
    browseButton.setBounds(bounds.removeFromTop(50).removeFromLeft(250));
    bounds.removeFromTop(30);
    
    dropZoneLabel.setBounds(bounds.removeFromTop(150));
    bounds.removeFromTop(20);
    
    sampleNameLabel.setBounds(bounds.removeFromTop(40));
    bounds.removeFromTop(20);
    
    auto sliderBounds = bounds.removeFromTop(50);
    sliderBounds.removeFromLeft(150); // Label space
    volumeSlider.setBounds(sliderBounds);
}

//==============================================================================
bool SamplerTab::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (const auto& file : files)
    {
        juce::String ext = juce::File(file).getFileExtension().toLowerCase();
        if (ext == ".wav" || ext == ".aiff" || ext == ".mp3" || ext == ".flac")
            return true;
    }
    return false;
}

void SamplerTab::filesDropped(const juce::StringArray& files, int x, int y)
{
    if (files.size() > 0)
    {
        juce::File sampleFile(files[0]);
        loadSampleFile(sampleFile);
    }
}

//==============================================================================
void SamplerTab::openFileBrowser()
{
    juce::FileChooser chooser("Load Sample",
                              juce::File::getSpecialLocation(juce::File::userMusicDirectory),
                              "*.wav;*.aiff;*.mp3;*.flac");
    
    if (chooser.browseForFileToOpen())
    {
        loadSampleFile(chooser.getResult());
    }
}

void SamplerTab::loadSampleFile(const juce::File& file)
{
    processor.sampleManager.loadSampleFile(file);
    sampleNameLabel.setText("📄 " + file.getFileName(), juce::dontSendNotification);
}
