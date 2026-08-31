#include "PluginEditor.h"

//==============================================================================
PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    // Create tabbed interface
    tabbedComponent = std::make_unique<TabbedComponent>(processorRef);
    addAndMakeVisible(*tabbedComponent);

    // Set editor size
    setSize(900, 600);
    setResizable(false, false);
}

PluginEditor::~PluginEditor()
{
}

//==============================================================================
void PluginEditor::paint(juce::Graphics& g)
{
    // Background
    g.fillAll(juce::Colour(0xff1a1a1a));
}

void PluginEditor::resized()
{
    // Fill entire editor with tabbed component
    if (tabbedComponent)
        tabbedComponent->setBounds(getLocalBounds());
}
