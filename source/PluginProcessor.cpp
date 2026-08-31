#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginProcessor::PluginProcessor()
    : AudioProcessor(BusesProperties()
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    , apvts(*this, nullptr, "Parameters", createParameterLayout())
    , presetManager(*this)
{
}

PluginProcessor::~PluginProcessor()
{
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Sampler Parameters
    params.push_back(std::make_unique<juce::AudioParameterInt>("sample", "Sample Select", 0, 10, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("pitch", "Pitch", -24.0f, 24.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("speed", "Speed", 0.5f, 2.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("attack", "Attack (ms)", 0.0f, 100.0f, 10.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("release", "Release (ms)", 10.0f, 1000.0f, 100.0f));

    // Bitcrusher Parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>("bitcrusher_depth", "Bitcrusher Depth", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("bitcrusher_mix", "Bitcrusher Mix", 0.0f, 1.0f, 0.0f));

    // Ring Modulator Parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ringmod_freq", "Ring Mod Frequency", 20.0f, 500.0f, 100.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ringmod_mix", "Ring Mod Mix", 0.0f, 1.0f, 0.0f));

    // Distortion Parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>("distortion_drive", "Distortion Drive", 0.0f, 10.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("distortion_tone", "Distortion Tone", 100.0f, 20000.0f, 5000.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("distortion_mix", "Distortion Mix", 0.0f, 1.0f, 0.0f));

    // Multiband EQ Parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>("multiband_low", "Multiband Low Gain", -12.0f, 12.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("multiband_mid", "Multiband Mid Gain", -12.0f, 12.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("multiband_high", "Multiband High Gain", -12.0f, 12.0f, 0.0f));

    // Reverb Parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>("reverb_room", "Reverb Room Size", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("reverb_damp", "Reverb Dampening", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("reverb_wet", "Reverb Wet Level", 0.0f, 1.0f, 0.3f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("reverb_dry", "Reverb Dry Level", 0.0f, 1.0f, 0.7f));

    // Master Parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>("master_gain", "Master Gain", -12.0f, 12.0f, 0.0f));

    return { params.begin(), params.end() };
}

//==============================================================================
void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    this->samplesPerBlock = samplesPerBlock;

    // Prepare DSP modules
    bitcrusher.prepare(sampleRate, samplesPerBlock);
    ringModulator.prepare(sampleRate, samplesPerBlock);
    distortion.prepare(sampleRate, samplesPerBlock);
    multiband.prepare(sampleRate, samplesPerBlock);
    reverb.prepare(sampleRate, samplesPerBlock);
    
    // Prepare sample manager
    sampleManager.prepare(sampleRate);
}

void PluginProcessor::releaseResources()
{
    // Clean up audio resources if needed
}

bool PluginProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // Stereo output support
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear unused channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Process MIDI events
    for (auto metadata : midiMessages)
    {
        auto msg = metadata.getMessage();
        if (msg.isNoteOn())
            sampleManager.triggerNote(msg.getNoteNumber(), msg.getVelocity());
        else if (msg.isNoteOff())
            sampleManager.releaseNote(msg.getNoteNumber());
    }

    // Get parameters
    float bitcrusherDepth = apvts.getRawParameterValue("bitcrusher_depth")->load();
    float bitcrusherMix = apvts.getRawParameterValue("bitcrusher_mix")->load();
    float ringModFreq = apvts.getRawParameterValue("ringmod_freq")->load();
    float ringModMix = apvts.getRawParameterValue("ringmod_mix")->load();
    float distDrive = apvts.getRawParameterValue("distortion_drive")->load();
    float distTone = apvts.getRawParameterValue("distortion_tone")->load();
    float distMix = apvts.getRawParameterValue("distortion_mix")->load();
    float multibandLow = apvts.getRawParameterValue("multiband_low")->load();
    float multibandMid = apvts.getRawParameterValue("multiband_mid")->load();
    float multibandHigh = apvts.getRawParameterValue("multiband_high")->load();
    float reverbRoom = apvts.getRawParameterValue("reverb_room")->load();
    float reverbDamp = apvts.getRawParameterValue("reverb_damp")->load();
    float reverbWet = apvts.getRawParameterValue("reverb_wet")->load();
    float reverbDry = apvts.getRawParameterValue("reverb_dry")->load();
    float masterGain = apvts.getRawParameterValue("master_gain")->load();

    // Configure DSP modules
    bitcrusher.setBitDepth(static_cast<int>(bitcrusherDepth * 15.0f) + 1);
    ringModulator.setFrequency(ringModFreq);
    distortion.setDrive(distDrive);
    distortion.setTone(distTone);
    multiband.setGain(0, multibandLow);
    multiband.setGain(1, multibandMid);
    multiband.setGain(2, multibandHigh);
    reverb.setRoomSize(reverbRoom);
    reverb.setDampening(reverbDamp);
    reverb.setWetLevel(reverbWet);
    reverb.setDryLevel(reverbDry);

    auto* channelData = buffer.getWritePointer(0);

    // Process audio block sample-by-sample
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        // Get sample from sampler engine
        float value = sampleManager.getNextSample();

        // Process through FX chain
        value = bitcrusher.process(value) * bitcrusherMix + value * (1.0f - bitcrusherMix);
        value = ringModulator.process(value) * ringModMix + value * (1.0f - ringModMix);
        value = distortion.process(value) * distMix + value * (1.0f - distMix);
        value = multiband.process(value);
        value = reverb.process(value);

        // Apply master gain
        float masterGainLinear = juce::Decibels::decibelsToGain(masterGain);
        value *= masterGainLinear;

        // Prevent denormalization and clipping
        if (std::abs(value) < 1e-10f)
            value = 0.0f;
        value = juce::jlimit(-1.0f, 1.0f, value);

        channelData[sample] = value;
    }

    // Copy to stereo
    if (getTotalNumOutputChannels() > 1)
    {
        auto* rightChannel = buffer.getWritePointer(1);
        juce::FloatVectorOperations::copy(rightChannel, channelData, buffer.getNumSamples());
    }
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor(*this);
}

//==============================================================================
void PluginProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyValueTree();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PluginProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
