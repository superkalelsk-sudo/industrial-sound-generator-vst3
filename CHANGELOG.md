# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned
- Polyphonic sample playback (multiple overlapping notes)
- ADSR envelope implementation
- Improved reverb with larger buffers
- Sample interpolation for smooth pitch-shifting
- Undo/Redo system
- MIDI learn for parameter assignment
- Theme customization (dark/light)

## [1.0.0] - 2026-08-31

### Added
- Initial VST3 plugin release
- Tabbed UI with 4 main sections:
  - **Sampler Tab:** Drag-drop sample loading, file browser
  - **Effects Tab:** Bitcrusher, Ring Modulator, Distortion, Multiband EQ, Reverb
  - **Synthesis Tab:** Pitch, Speed, Attack, Release controls
  - **Master Tab:** Output gain, Preset management, CPU monitoring
- DSP Effect Chain:
  - Bitcrusher (1-16 bit digital degradation)
  - Ring Modulator (20-500 Hz carrier)
  - Industrial Distortion (soft-clipping with tone control)
  - Multiband EQ (3-band parametric)
  - Freeverb Reverberator (Schroeder reverb)
- MIDI note triggering with velocity sensitivity
- Preset save/load system
- Full cross-platform support (Windows, macOS, Linux)
- Comprehensive documentation:
  - Architecture guide
  - DSP algorithm explanations
  - Build instructions for all platforms

### Technical
- Built with JUCE 7.x framework
- C++17 standard
- Real-time safe audio processing (<20% CPU)
- Lock-free parameter synchronization
- VST3 plugin format (3.7+)

---

## Version History Format

### [X.Y.Z] - YYYY-MM-DD

#### Added
- New features

#### Changed
- Changes in existing functionality

#### Fixed
- Bug fixes

#### Deprecated
- Deprecated features

#### Removed
- Removed features

#### Security
- Security vulnerability fixes
