#pragma once

#include <JuceHeader.h>
#include "dsp/Bitcrusher.h"
#include "dsp/RingModulator.h"
#include "dsp/IndustrialDistortion.h"
#include "dsp/MultibandEQ.h"
#include "dsp/ReverbEngine.h"
#include "utilities/SampleManager.h"
#include "utilities/PresetManager.h"

//==============================================================================
/**
    PluginProcessor - Main Audio Processing Engine
    
    Handles:
    - MIDI input and note triggering
    - Sample playback and management
    - DSP effect chain processing
    - Parameter management via AudioProcessorValueTreeState
    - Preset save/load functionality
*/
class PluginProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    PluginProcessor();
    ~PluginProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    //==============================================================================
    const juce::String getName() const override { return "Industrial Generator"; }

    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 2.0; }

    //==============================================================================
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================
    // Parameter Management
    juce::AudioProcessorValueTreeState apvts;
    
    // DSP Modules
    Bitcrusher bitcrusher;
    RingModulator ringModulator;
    IndustrialDistortion distortion;
    MultibandEQ multiband;
    ReverbEngine reverb;
    
    // Utilities
    SampleManager sampleManager;
    PresetManager presetManager;
    
    // Parameter IDs (for automation)
    enum ParameterID
    {
        SampleSelectId = 0,
        PitchId = 1,
        SpeedId = 2,
        AttackId = 3,
        ReleaseId = 4,
        
        BitcrusherDepthId = 10,
        BitcrusherMixId = 11,
        
        RingModFreqId = 20,
        RingModMixId = 21,
        
        DistortionDriveId = 30,
        DistortionToneId = 31,
        DistortionMixId = 32,
        
        MultibandLowGainId = 40,
        MultibandMidGainId = 41,
        MultibandHighGainId = 42,
        
        ReverbRoomId = 50,
        ReverbDampId = 51,
        ReverbWetId = 52,
        ReverbDryId = 53,
        
        MasterGainId = 100,
    };

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    // Audio processing state
    double currentSampleRate = 44100.0;
    int samplesPerBlock = 512;
    
    // Performance monitoring
    juce::Atomic<float> cpuLoad { 0.0f };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
