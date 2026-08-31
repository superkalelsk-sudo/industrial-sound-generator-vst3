# Build Guide - Industrial Sound Generator VST3

Complete step-by-step guide to building the plugin on macOS, Windows, and Linux.

---

## Prerequisites

### All Platforms
1. **C++17 Compiler**
   - macOS: Xcode Command Line Tools
   - Windows: Visual Studio 2019 or later (with C++ workload)
   - Linux: GCC 9+ or Clang 10+

2. **CMake 3.15+**
   ```bash
   # macOS (via Homebrew)
   brew install cmake
   
   # Windows: Download from cmake.org
   # Linux
   sudo apt-get install cmake
   ```

3. **Git**
   ```bash
   git clone https://github.com/superkalelsk-sudo/industrial-sound-generator-vst3.git
   cd industrial-sound-generator-vst3
   ```

### macOS Specific
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Homebrew packages
brew install cmake git
```

### Windows Specific
- Visual Studio 2019+ (Community edition is fine)
- Windows 10 SDK (installed with Visual Studio)
- Git for Windows

### Linux Specific
```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake git libx11-dev libxinerama-dev libxrandr-dev libxcursor-dev mesa-common-dev

# Fedora
sudo dnf install gcc-c++ cmake git libX11-devel libXinerama-devel libXrandr-devel libXcursor-devel mesa-libGL-devel
```

---

## Step 1: Clone Repository

```bash
git clone https://github.com/superkalelsk-sudo/industrial-sound-generator-vst3.git
cd industrial-sound-generator-vst3
```

---

## Step 2: Download JUCE Framework

### Option A: Via Git Submodule (Recommended)
```bash
git submodule update --init --recursive
# JUCE will be in: ./JUCE/
```

### Option B: Manual Download
1. Visit [juce.com/download](https://juce.com/download)
2. Download JUCE 7.x or later
3. Extract to `./JUCE/` folder in project root

Verify:
```bash
ls -la JUCE/
# Should see: CMakeLists.txt, modules/, etc.
```

---

## Step 3: Configure Build

### Create Build Directory
```bash
mkdir build
cd build
```

### Generate Build Files

**macOS (Xcode)**
```bash
cmake .. -G Xcode \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13
```

**Windows (Visual Studio)**
```bash
cmake .. -G "Visual Studio 16 2019" -A x64 ^
  -DCMAKE_BUILD_TYPE=Release
```

**Linux (Unix Makefiles)**
```bash
cmake .. -G "Unix Makefiles" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_COMPILER=g++
```

### CMakeLists.txt Breakdown

The project uses this CMake structure:

```cmake
cmake_minimum_required(VERSION 3.15)
project(IndustrialGenerator VERSION 1.0.0 LANGUAGES CXX)

# Add JUCE as subdirectory
add_subdirectory(JUCE)

# Create VST3 plugin target
juce_add_plugin(IndustrialGenerator
    VERSION 1.0.0
    DESCRIPTION "Industrial Sound Generator VST3"
    PLUGIN_MANUFACTURER_CODE "IGVS"
    PLUGIN_CODE "IGN0"
    FORMATS VST3
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE)

# Source files
target_sources(IndustrialGenerator PRIVATE
    source/PluginProcessor.cpp
    source/PluginEditor.cpp
    source/dsp/Bitcrusher.cpp
    source/dsp/RingModulator.cpp
    source/dsp/IndustrialDistortion.cpp
    source/dsp/MultibandEQ.cpp
    source/dsp/ReverbEngine.cpp
    source/ui/TabbedComponent.cpp
    source/ui/SamplerTab.cpp
    source/ui/EffectsTab.cpp
    source/ui/SynthesisTab.cpp
    source/ui/MasterTab.cpp
    source/utilities/SampleManager.cpp
    source/utilities/PresetManager.cpp)

# Link JUCE
target_link_libraries(IndustrialGenerator PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_gui_extra)
```

---

## Step 4: Build

### macOS
```bash
cd build
cmake --build . --config Release --target IndustrialGenerator_VST3

# Plugin location:
# build/IndustrialGenerator_artefacts/VST3/IndustrialGenerator.vst3
```

### Windows
```bash
cd build
cmake --build . --config Release

# Plugin location:
# build\IndustrialGenerator_artefacts\VST3\IndustrialGenerator.vst3
```

### Linux
```bash
cd build
cmake --build . --config Release

# Plugin location:
# build/IndustrialGenerator_artefacts/VST3/IndustrialGenerator.vst3
```

**Build Time:** ~5-15 minutes (first build is slowest due to JUCE compilation)

---

## Step 5: Install to DAW Plugin Path

### macOS
```bash
cp -r build/IndustrialGenerator_artefacts/VST3/IndustrialGenerator.vst3 \
  ~/Library/Audio/Plug-Ins/VST3/

# Verify
ls ~/Library/Audio/Plug-Ins/VST3/ | grep Industrial
```

### Windows
```bash
xcopy build\IndustrialGenerator_artefacts\VST3\IndustrialGenerator.vst3 ^
  "%ProgramFiles%\Common Files\VST3\" /E /I /Y

# Verify in Command Prompt:
dir "%ProgramFiles%\Common Files\VST3\IndustrialGenerator.vst3"
```

### Linux
```bash
cp -r build/IndustrialGenerator_artefacts/VST3/IndustrialGenerator.vst3 \
  ~/.vst3/

# Verify
ls ~/.vst3/ | grep Industrial
```

---

## Step 6: Rescan in DAW

Most DAWs need to rescan plugins after installation:

| DAW | How to Rescan |
|-----|--------------|
| **Ableton Live** | Preferences → File Handling → Plugins → Rescan |
| **FL Studio** | Options → Manage Plugins → Rescan plugin folders |
| **Studio One** | Options → Locations → Plug-in Manager → Rescan |
| **Reaper** | Options → Show REAPER resource path in explorer, rescan |
| **Bitwig** | Settings → Plug-ins → Rescan |
| **Logic Pro** | Preferences → Plugins → Manage... → Rescan |

---

## Troubleshooting

### Build Fails: "JUCE not found"
**Solution:** Ensure JUCE is in `./JUCE/` directory
```bash
# Check
ls JUCE/CMakeLists.txt
# If missing, download from juce.com or use git submodule
```

### Build Fails: "CMAKE not found"
**Solution:** Install CMake
```bash
# macOS
brew install cmake

# Windows: Download from cmake.org

# Linux
sudo apt-get install cmake
```

### Build Fails: "C++ compiler error"
**Solution:** Ensure C++17 support
```bash
# macOS: Xcode 10+
# Windows: Visual Studio 2017+
# Linux: GCC 9+ or Clang 10+
```

### Plugin Won't Load in DAW
1. Verify plugin file exists: `ls ~/Library/Audio/Plug-Ins/VST3/` (macOS)
2. Check DAW log file for errors
3. Try running VST3 validator:
   ```bash
   # JUCE includes validator tool
   ./build/IndustrialGenerator_VST3 --validate
   ```

### macOS: "Plugin from unidentified developer"
**Solution:** Approve in System Preferences
1. System Preferences → Security & Privacy
2. Click "Allow" for IndustrialGenerator.vst3

### Windows: "Plugin failed to load"
**Solution:** Ensure Visual C++ Redistributable is installed
```bash
# Download from Microsoft:
# https://support.microsoft.com/en-us/help/2977003/
```

---

## Development Build (Debug)

For iterative development with debug symbols:

```bash
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .

# Debug builds are larger, slower, but have symbols for debugging
```

### Attach Debugger

**macOS (Xcode)**
```bash
open IndustrialGenerator.xcodeproj
# Build → Run, then set breakpoints in Xcode
```

**Windows (Visual Studio)**
```bash
start IndustrialGenerator.sln
# Debug → Start Debugging (F5)
```

**Linux (GDB)**
```bash
gdb ./build/IndustrialGenerator_artefacts/VST3/IndustrialGenerator.vst3
(gdb) break PluginProcessor.cpp:42
(gdb) run
```

---

## Clean Build

To start fresh (remove all compiled files):

```bash
rm -rf build
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

---

## Build Optimization

For fastest performance build:

### All Platforms
```bash
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS="-O3 -march=native"
```

### macOS Additional
```bash
-DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"  # Universal binary (Apple Silicon + Intel)
```

### Windows Additional
```bash
-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDLL
```

---

## Continuous Integration (GitHub Actions)

The project includes `.github/workflows/build.yml` for automated builds:

```yaml
name: Build VST3 Plugin

on: [push, pull_request]

jobs:
  build:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [macos-latest, windows-latest, ubuntu-latest]
    steps:
      - uses: actions/checkout@v2
        with:
          submodules: recursive
      
      - name: Build
        run: |
          mkdir build
          cd build
          cmake .. -DCMAKE_BUILD_TYPE=Release
          cmake --build .
      
      - name: Upload Plugin
        uses: actions/upload-artifact@v2
        with:
          name: IndustrialGenerator-${{ matrix.os }}
          path: build/IndustrialGenerator_artefacts/VST3/
```

---

## Next Steps After Build

1. ✅ **Open your DAW**
2. ✅ **Insert Industrial Sound Generator as VST3**
3. ✅ **Load a sample** via Sampler tab drag-drop
4. ✅ **Send MIDI notes** to trigger samples
5. ✅ **Shape sound** with Effects tab
6. ✅ **Save preset** in Master tab

---

## Getting Help

- Check build logs: `cmake --build . --verbose`
- VST3 validator: Run the plugin with `--validate` flag
- GitHub Issues: [Report build problems](https://github.com/superkalelsk-sudo/industrial-sound-generator-vst3/issues)
- JUCE Documentation: [juce.com/learn](https://juce.com/learn)

---

**Status:** Build guide last updated 2026-08-31
