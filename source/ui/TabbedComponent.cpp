#include "TabbedComponent.h"
#include "SamplerTab.h"
#include "EffectsTab.h"
#include "SynthesisTab.h"
#include "MasterTab.h"
#include "../PluginProcessor.h"

//==============================================================================
TabbedComponent::TabbedComponent(PluginProcessor& p)
    : processor(p),
      tabBar(juce::TabbedButtonBar::TabsAtTop)
{
    // Add tabs to the button bar
    tabBar.addTab("Sampler", juce::Colour(0xff2e2e2e), 0);
    tabBar.addTab("Effects", juce::Colour(0xff2e2e2e), 1);
    tabBar.addTab("Synthesis", juce::Colour(0xff2e2e2e), 2);
    tabBar.addTab("Master", juce::Colour(0xff2e2e2e), 3);
    
    tabBar.setOutlineThickness(2);
    tabBar.addListener(this);
    addAndMakeVisible(tabBar);

    // Create tab components
    samplerTab = std::make_unique<SamplerTab>(processor);
    effectsTab = std::make_unique<EffectsTab>(processor);
    synthesisTab = std::make_unique<SynthesisTab>(processor);
    masterTab = std::make_unique<MasterTab>(processor);

    // Show first tab by default
    currentTab = std::move(samplerTab);
    addAndMakeVisible(*currentTab);
}

TabbedComponent::~TabbedComponent()
{
}

//==============================================================================
void TabbedComponent::paint(juce::Graphics& g)
{
    // Dark background
    g.fillAll(juce::Colour(0xff1a1a1a));
}

void TabbedComponent::resized()
{
    auto bounds = getLocalBounds();
    
    // Tab bar at top
    tabBar.setBounds(bounds.removeFromTop(40));
    
    // Current tab takes rest of space
    if (currentTab)
        currentTab->setBounds(bounds);
}

void TabbedComponent::currentTabChanged(int newCurrentTabIndex, const juce::String& newCurrentTabName)
{
    removeChildComponent(currentTab.get());
    
    switch (newCurrentTabIndex)
    {
        case 0:
            samplerTab = std::make_unique<SamplerTab>(processor);
            currentTab = std::move(samplerTab);
            break;
        case 1:
            effectsTab = std::make_unique<EffectsTab>(processor);
            currentTab = std::move(effectsTab);
            break;
        case 2:
            synthesisTab = std::make_unique<SynthesisTab>(processor);
            currentTab = std::move(synthesisTab);
            break;
        case 3:
            masterTab = std::make_unique<MasterTab>(processor);
            currentTab = std::move(masterTab);
            break;
        default:
            break;
    }
    
    addAndMakeVisible(*currentTab);
    resized();
}
