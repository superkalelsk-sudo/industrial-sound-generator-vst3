# Contributing to Industrial Sound Generator VST3

Thank you for your interest in contributing! This document provides guidelines for contributing to the project.

## Getting Started

1. **Fork the repository** on GitHub
2. **Clone your fork** locally
   ```bash
   git clone https://github.com/YOUR_USERNAME/industrial-sound-generator-vst3.git
   cd industrial-sound-generator-vst3
   ```
3. **Create a feature branch**
   ```bash
   git checkout -b feature/your-feature-name
   ```

## Development Workflow

### Building
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

### Code Style
- Use **4-space indentation**
- Follow **JUCE naming conventions** (camelCase for variables, PascalCase for classes)
- Keep lines under **120 characters** where possible
- Comment complex DSP algorithms

### Testing

1. **Manual Testing**
   - Test in at least 2 DAWs (Ableton, FL Studio, Studio One, etc.)
   - Test on Windows, macOS, and Linux if possible
   - Verify audio produces no clicks/pops
   - Check CPU usage stays under 20%

2. **VST3 Validation**
   ```bash
   # Use JUCE's built-in validator
   ./build/IndustrialGenerator_VST3 --validate
   ```

## Commit Guidelines

- **Atomic commits:** Each commit should be a single logical change
- **Clear messages:** Use present tense ("Add feature" not "Added feature")
- **Reference issues:** Use `Fixes #123` in commit messages when applicable

Example:
```
Add ring modulator frequency control slider

- Add slider to Effects tab
- Update parameter layout
- Implement in EffectsTab.cpp

Fixes #45
```

## Pull Request Process

1. **Push to your fork**
   ```bash
   git push origin feature/your-feature-name
   ```

2. **Open a Pull Request** on GitHub with:
   - Clear title describing the change
   - Description of what changed and why
   - Any relevant issue numbers (Closes #123)
   - Screenshots/audio samples for UI changes

3. **Code Review**
   - Respond to review comments
   - Make requested changes in new commits (don't force-push)
   - Re-request review when ready

4. **Merge**
   - Maintainers will merge when approved
   - Squash and merge for feature branches

## Areas for Contribution

### High Priority
- [ ] Complete sample playback interpolation (pitch-shifting)
- [ ] Implement polyphonic playback (multiple notes)
- [ ] Add envelope ADSR implementation
- [ ] Improve reverb algorithm (larger buffer sizes)

### Medium Priority
- [ ] Add preset browser UI
- [ ] Implement undo/redo
- [ ] Add MIDI learn for parameters
- [ ] Performance optimizations (SIMD)

### Low Priority
- [ ] Dark/light theme support
- [ ] Custom preset categories
- [ ] Audio export functionality
- [ ] Plugin analytics

## Reporting Issues

Use GitHub Issues to report bugs. Include:
1. **System info** (OS, DAW, JUCE version)
2. **Steps to reproduce**
3. **Expected behavior**
4. **Actual behavior**
5. **Audio/screenshots** (if relevant)

## Feature Requests

Feature requests are welcome! Please include:
1. **Use case** (why you need it)
2. **Proposed solution** (how it should work)
3. **Alternatives** (other ways to achieve it)

## Questions?

- Check [ARCHITECTURE.md](./ARCHITECTURE.md) for plugin architecture
- Check [DSP_ALGORITHMS.md](./docs/DSP_ALGORITHMS.md) for effect explanations
- Open a GitHub Discussion for questions

---

Thank you for contributing! 🎵
