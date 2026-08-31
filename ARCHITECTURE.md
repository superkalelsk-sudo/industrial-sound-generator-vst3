# VST3 Architecture & Plugin Fundamentals

This document explains the core architecture of VST3 plugins and how the Industrial Sound Generator implements them.

## VST3 Plugin Lifecycle

### 1. Plugin Initialization

When a DAW loads the plugin:

```
DAW loads .vst3 bundle
    ↓
Plugin entry point (usually defined by JUCE)
    ↓
DAW instantiates IComponent interface
    ↓
PluginProcessor created (audio processing engine)
    ↓
DAW instantiates IEditController interface
    ↓
PluginEditor created (GUI)
    ↓
DAW connects them via IConnectionPoint (parameter sync)
    ↓
Plugin ready for audio/MIDI input
```

### 2. Audio Processing Thread

The audio thread runs independently and processes samples in **blocks** (typically 64-2048 samples):

```cpp
// This is called every audio block (~2.3ms at 44.1kHz)
void PluginProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    // 1. Process MIDI events (note on/off, CC, pitch bend)
    for (auto metadata : midiMessages)
    {
        if (metadata.data[0] & 0x90) // Note On
            samplerEngine.triggerSample(metadata.data[1]); // MIDI note
    }
    
    // 2. Process audio samples
    auto* channelData = buffer.getWritePointer(0);
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        // Fetch sample from sampler
        float value = samplerEngine.getNextSample();
        
        // Route through FX chain
        value = bitcrusher.process(value);
        value = ringModulator.process(value);
        value = distortion.process(value);
        value = multiband.process(value);
        value = reverb.process(value);
        
        // Output
        channelData[sample] = value * masterGain;
    }
}
```

**Key Rules:**
- **Real-time safe:** No memory allocation, no I/O, no blocking operations
- **No GUI calls:** GUI runs on separate thread
- **Lock-free parameter updates:** Parameters updated via atomic variables

### 3. Parameter Management

VST3 uses a **parameter ID system** for automation and state saving:

```cpp
// Define parameters (in PluginProcessor.cpp)
enum ParameterID
{
    SampleSelectId = 0,
    PitchId = 1,
    SpeedId = 2,
    BitcrusherDepthId = 3,
    RingModFreqId = 4,
    DistortionDriveId = 5,
    ReverbWetId = 6,
    MasterGainId = 7,
};

// Create parameter layout
AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<AudioParameterFloat>(
        "sample", "Sample Select", 0.0f, 10.0f, 0.0f));
    
    params.push_back(std::make_unique<AudioParameterFloat>(
        "pitch", "Pitch", -12.0f, 12.0f, 0.0f));
    
    params.push_back(std::make_unique<AudioParameterFloat>(
        "bitcrusher", "Bitcrusher Depth", 0.0f, 1.0f, 0.0f));
    
    // ... more parameters
    
    return {params.begin(), params.end()};
}
```

### 4. GUI Update Mechanism

The GUI runs on the **message thread** and communicates with the audio thread safely:

```
Audio Thread (Real-Time)          Message Thread (GUI)
─────────────────────────────────────────────────────
    ↓
Updates atomic parameter value
    ↓ (lock-free queue)
                                    Reads atomic value
                                    ↓
                                    Updates slider position
                                    ↓
User moves slider
                                    ↓
                                Sends value to parameter
                                    ↓ (lock-free queue)
    ↓
Reads new parameter value
    ↓
Uses in audio processing
```

---

## Plugin Components

### PluginProcessor (Audio Engine)

**File:** `source/PluginProcessor.h/cpp`

Handles:
- Sample loading and management
- MIDI note triggering
- Audio buffer processing
- Parameter value lookup during processing
- State save/load (presets)

```cpp
class PluginProcessor : public AudioProcessor
{
public:
    PluginProcessor();
    ~PluginProcessor();
    
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioBuffer<float>&, MidiBuffer&) override;
    
    // Parameter management
    AudioProcessorValueTreeState apvts;
    
    // DSP modules
    SampleManager samplerEngine;
    Bitcrusher bitcrusher;
    RingModulator ringModulator;
    IndustrialDistortion distortion;
    MultibandEQ multiband;
    ReverbEngine reverb;
};
```

### PluginEditor (GUI)

**File:** `source/PluginEditor.h/cpp`

Handles:
- Window creation and sizing
- Tabbed component management
- Parameter binding to UI controls
- Drag-drop zones
- Real-time meter display

```cpp
class PluginEditor : public AudioProcessorEditor
{
public:
    PluginEditor(PluginProcessor&);
    ~PluginEditor();
    
    void paint(Graphics& g) override;
    void resized() override;
    
private:
    PluginProcessor& processor;
    
    // Tabbed UI
    TabbedComponent tabs;
    std::unique_ptr<SamplerTab> samplerTab;
    std::unique_ptr<EffectsTab> effectsTab;
    std::unique_ptr<SynthesisTab> synthesisTab;
    std::unique_ptr<MasterTab> masterTab;
};
```

---

## Tabbed Interface Architecture

### Tab Structure

Each tab is a JUCE Component that manages its own controls:

```
┌─────────────────────────────────────────────────────┐
│  Industrial Sound Generator                         │
├─────────────────────────────────────────────────────┤
│ [Sampler] [Effects] [Synthesis] [Master]            │
├─────────────────────────────────────────────────────┤
│                                                      │
│  ┌─ Sampler Tab ────────────────────────────────┐  │
│  │ [Browse] [Drag-Drop Area]  [Waveform View]  │  │
│  │ ┌────────────────────────────────────────┐  │  │
│  │ │ industrial_kick_001.wav                │  │  │
│  │ │ [████████████░░░░░░░░░░░░░░░░░░░░░░] │  │  │
│  │ │ Duration: 0.5s | Sample Rate: 44.1kHz │  │  │
│  │ │ [▶ Preview]                            │  │  │
│  │ └────────────────────────────────────────┘  │  │
│  └────────────────────────────────────────────────┘  │
│                                                      │
└─────────────────────────────────────────────────────┘
```

### Tab Implementation Pattern

```cpp
class SamplerTab : public Component, public FileDragAndDropTarget
{
public:
    SamplerTab(PluginProcessor& p, AudioProcessorValueTreeState& apvts);
    
    void paint(Graphics& g) override;
    void resized() override;
    
    // Drag-drop interface
    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;
    
private:
    PluginProcessor& processor;
    AudioProcessorValueTreeState& apvts;
    
    TextButton browseButton;
    WaveformDisplay waveformViewer;
    Label sampleNameLabel;
    Slider volumeSlider;
    
    void openFileBrowser();
    void loadSampleFile(const File& file);
};
```

### Four Main Tabs

#### **Sampler Tab**
- Drag-drop zone for loading samples
- File browser button
- Waveform visualization
- Sample metadata (duration, sample rate)
- Volume/pan controls
- Preview playback

#### **Effects Tab**
- Bitcrusher controls (bit depth 1-16)
- Ring Modulator (frequency, mix)
- Distortion (drive, tone)
- Multiband EQ (3 bands with frequency/gain/Q)
- Reverb (room size, dampening, wet/dry mix)

#### **Synthesis Tab**
- Pitch transposition (-24 to +24 semitones)
- Playback speed (0.5x to 2.0x)
- Attack/Release envelope
- Velocity sensitivity
- Pitch bend range

#### **Master Tab**
- Output level meter (VU meter)
- Master gain control
- Preset browser/save/load
- CPU meter (for monitoring DSP load)
- Settings (polyphony, sample bit depth)

---

## Audio DSP Signal Flow

```
MIDI In
   ↓
[Sampler Engine] ← Loads sample based on MIDI note
   ↓
[Bitcrusher] ← Reduce bit depth for grit
   ↓
[Ring Modulator] ← Add metallic modulation
   ↓
[Distortion] ← Aggressive saturation
   ↓
[Multiband EQ] ← Surgical frequency shaping
   ↓
[Reverb] ← Add space/ambience
   ↓
[Master Gain] ← Volume control
   ↓
Output → DAW Output Buffer
```

Each module processes **sample-by-sample** in real-time.

---

## Thread Safety

VST3/JUCE handle thread safety for you, but here's what happens behind the scenes:

### Audio Thread (Real-Time, ~1kHz frequency)
```cpp
void processBlock(...) // Called from audio thread
{
    auto gain = *gainParameter.getValuePointer(); // Atomic read - safe!
    // ... process audio with gain
}
```

### GUI Thread (User interaction, event-driven)
```cpp
void sliderValueChanged(Slider* slider)
{
    gainParameter.setValueNotifyingHost(slider->getValue()); // Thread-safe!
}
```

**Key:** JUCE's `AudioProcessorValueTreeState` uses **lock-free queues** and **atomic variables** to synchronize parameters between threads without blocking the audio thread.

---

## State Management (Presets)

### Save Preset
```cpp
void PluginProcessor::getStateInformation(MemoryBlock& destData)
{
    auto state = apvts.copyValueTree();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}
```

### Load Preset
```cpp
void PluginProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    auto xmlState = getXmlFromBinary(data, sizeInBytes);
    auto valueTree = ValueTree::fromXml(*xmlState);
    apvts.replaceState(valueTree);
}
```

Presets include:
- Sample selection
- All parameter values
- FX chain configuration
- Preset name/metadata

---

## Performance Considerations

### CPU Budget
- **Real-time constraint:** ~5-10ms per block at 44.1kHz
- **Target:** <10% CPU for plugin

### Optimization Strategies
1. **Vectorization:** Use SIMD operations (SSE/AVX) for DSP
2. **Buffer caching:** Pre-allocate all memory in `prepareToPlay()`
3. **Parameter smoothing:** Ramp parameter changes over 64 samples to avoid clicks
4. **Efficient resampling:** Use high-quality interpolation for pitch shifting

### Memory Model
```
Audio Thread Memory (Stack-based, pre-allocated)
├── Processing buffers
├── DSP state (filter coefficients, reverb state)
└── Parameter cache (read once per block)

GUI Thread Memory (Heap-based, dynamic)
├── UI components
├── Slider/button state
└── File browser
```

---

## Debugging & Testing

### VST3 Validator
```bash
# JUCE provides VST3 validation tools
./VST3PluginTestHost IndustrialGenerator.vst3
```

### Common Issues

| Issue | Cause | Fix |
|-------|-------|-----|
| Audio clicks/pops | Parameter changes mid-block | Use parameter smoothing over 64 samples |
| Latency spikes | Memory allocation in audio thread | Pre-allocate all buffers |
| Automation doesn't work | Parameter ID mismatch | Verify parameter IDs in layout |
| Drag-drop doesn't work | Component not accepting drops | Implement `FileDragAndDropTarget` interface |
| Preset not saving | State not in `ValueTree` | Bind all controls to `apvts` |

---

## References

- [VST3 Official SDK](https://github.com/steinbergmedia/vst3sdk)
- [JUCE Audio Plugin Framework](https://juce.com/discover/juce)
- [Real-Time Audio Programming 101](https://www.rossbencina.com/)
