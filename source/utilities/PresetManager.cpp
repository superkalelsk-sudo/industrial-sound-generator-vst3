#include "PresetManager.h"
#include "../PluginProcessor.h"

PresetManager::PresetManager(PluginProcessor& p)
    : processor(p)
{
    // Initialize preset directory
    presetDirectory = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
        .getChildFile("IndustrialGenerator")
        .getChildFile("Presets");
    
    presetDirectory.createDirectory();
}

void PresetManager::savePreset(const juce::String& presetName)
{
    juce::File presetFile = presetDirectory.getChildFile(presetName + ".xml");
    
    auto state = processor.apvts.copyValueTree();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    
    xml->writeTo(presetFile);
}

void PresetManager::loadPreset(const juce::String& presetName)
{
    juce::File presetFile = presetDirectory.getChildFile(presetName + ".xml");
    
    if (presetFile.existsAsFile())
    {
        std::unique_ptr<juce::XmlElement> xml(juce::XmlDocument::parse(presetFile));
        if (xml != nullptr)
        {
            auto state = juce::ValueTree::fromXml(*xml);
            processor.apvts.replaceState(state);
        }
    }
}

juce::StringArray PresetManager::getPresetNames() const
{
    juce::StringArray names;
    
    auto presets = presetDirectory.findChildFiles(juce::File::findFiles, false, "*.xml");
    for (const auto& preset : presets)
    {
        names.add(preset.getFileNameWithoutExtension());
    }
    
    return names;
}
