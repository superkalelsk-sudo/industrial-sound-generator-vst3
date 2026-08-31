# Industrial Sound Generator VST3 Plugin

A professional-grade VST3 audio plugin for generating and processing industrial sounds with one-shot sample loading and drag-and-drop support.

## Features

### 🎵 Sample Management
- Drag-and-drop sample loading directly into the plugin
- Support for WAV, AIFF, MP3, and FLAC formats
- One-shot sample triggering via MIDI
- Real-time sample browser with waveform preview

### 🎛️ Industrial FX Chain
- **Bitcrusher** — reduce bit depth for gritty, digital textures
- **Ring Modulator** — metallic, bell-like tones
- **Distortion** — aggressive harmonic saturation
- **Multi-band EQ** — surgical frequency shaping
- **Reverb** — industrial space/ambience

### 🎮 Tabbed UI Architecture
The plugin features a clean, organized tabbed interface:
- **Sampler Tab** — Load, preview, and manage samples
- **Effects Tab** — Configure industrial FX chain
- **Synthesis Tab** — Pitch, speed, envelope controls
- **Master Tab** — Output levels, metering, presets

### 🔧 Technical
- **VST3** architecture for cross-DAW compatibility
- Built with **JUCE Framework** (C++)
- Full parameter automation support
- Preset save/load system
- Real-time CPU-efficient DSP

## Project Structure

```
industrial-sound-generator-vst3/
├── README.md                          # This file
├── ARCHITECTURE.md                    # VST3 & plugin lifecycle docs
├── CMakeLists.txt                     # Build configuration
├── juce_modules/                      # JUCE framework
├── source/
│   ├── PluginProcessor.h              # Main audio processing
│   ├── PluginProcessor.cpp
│   ├── PluginEditor.h                 # GUI & tabbed interface
│   ├── PluginEditor.cpp
│   ├── dsp/
│   │   ├── Bitcrusher.h               # Industrial FX
│   │   ├── RingModulator.h
│   │   ├── IndustrialDistortion.h
│   │   ├── MultibanEqualize.h
│   │   └── ReverbEngine.h
│   ├── ui/
│   │   ├── TabbedComponent.h          # Tab management
│   │   ├── SamplerTab.h               # Sample UI
│   │   ├── EffectsTab.h               # FX controls
│   │   ├── SynthesisTab.h             # Synth controls
│   │   └── MasterTab.h                # Output/presets
│   └── utilities/
│       ├── SampleManager.h            # Sample loading
│       ├── PresetManager.h            # Preset handling
│       └── DragDropTarget.h           # Drag-drop impl
├── docs/
│   ├── VST3_FUNDAMENTALS.md           # Architecture guide
│   ├── DSP_ALGORITHMS.md              # Effect explanations
│   └── BUILD_GUIDE.md                 # Setup instructions
└── examples/
    └── sample_preset.json             # Example preset
```

## Quick Start

### Prerequisites
- C++17 compiler (MSVC, GCC, or Clang)
- CMake 3.15+
- JUCE 7.x or later
- Your favorite DAW (Ableton, FL Studio, Studio One, etc.)

### Build

```bash
# Clone the repository
git clone https://github.com/superkalelsk-sudo/industrial-sound-generator-vst3.git
cd industrial-sound-generator-vst3

# Create build directory
mkdir build
cd build

# Configure and build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .

# Plugin will be in: build/IndustrialGenerator_artefacts/VST3/
```

### Installation

**macOS:**
```bash
cp -r build/IndustrialGenerator_artefacts/VST3/IndustrialGenerator.vst3 \
  ~/Library/Audio/Plug-Ins/VST3/
```

**Windows:**
```bash
xcopy build\IndustrialGenerator_artefacts\VST3\IndustrialGenerator.vst3 ^
  "%ProgramFiles%\Common Files\VST3\" /E /I
```

**Linux:**
```bash
cp -r build/IndustrialGenerator_artefacts/VST3/IndustrialGenerator.vst3 \
  ~/.vst3/
```

## Usage

1. **Load Plugin** — Insert into VST3 slot in your DAW
2. **Load Sample** — Drag WAV/AIFF file into Sampler tab or click "Browse"
3. **Trigger** — Send MIDI notes to trigger sample playback
4. **Shape Sound** — Use Effects tab to add industrial character
5. **Save Preset** — Master tab to save/load your settings

## Documentation

- **[ARCHITECTURE.md](./ARCHITECTURE.md)** — Deep dive into VST3 plugin lifecycle, parameter management, and audio thread safety
- **[DSP_ALGORITHMS.md](./docs/DSP_ALGORITHMS.md)** — Explanation of each industrial effect with pseudocode
- **[BUILD_GUIDE.md](./docs/BUILD_GUIDE.md)** — Step-by-step build setup for all platforms

## Contributing

Issues and pull requests welcome! Please see CONTRIBUTING.md for guidelines.

## License

MIT License — see LICENSE file

## Resources

- [VST3 Official Documentation](https://steinbergmedia.github.io/vst3_dev_portal/)
- [JUCE Framework](https://juce.com/)
- [Audio Programming for Dummies](https://en.wikibooks.org/wiki/Digital_Audio_Processing)

---

**Status:** Alpha (Under Active Development)  
**Last Updated:** 2026-08-31
