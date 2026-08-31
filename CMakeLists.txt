cmake_minimum_required(VERSION 3.15)

project(IndustrialGenerator VERSION 1.0.0 LANGUAGES CXX)

# Add JUCE framework
add_subdirectory(JUCE)

# Main VST3 plugin target
juce_add_plugin(IndustrialGenerator
    VERSION 1.0.0
    DESCRIPTION "Industrial Sound Generator VST3 Plugin"
    HOMEPAGE_URL "https://github.com/superkalelsk-sudo/industrial-sound-generator-vst3"
    SUPPORT_MORE_THAN_TWO_CHANNELS FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    PLUGIN_MANUFACTURER_CODE "IGVS"
    PLUGIN_CODE "IGN0"
    FORMATS VST3
    PRODUCT_NAME "Industrial Generator"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE)

# Source files - Plugin Core
target_sources(IndustrialGenerator PRIVATE
    source/PluginProcessor.h
    source/PluginProcessor.cpp
    source/PluginEditor.h
    source/PluginEditor.cpp)

# Source files - DSP Modules
target_sources(IndustrialGenerator PRIVATE
    source/dsp/Bitcrusher.h
    source/dsp/RingModulator.h
    source/dsp/IndustrialDistortion.h
    source/dsp/MultibandEQ.h
    source/dsp/ReverbEngine.h)

# Source files - UI Components
target_sources(IndustrialGenerator PRIVATE
    source/ui/TabbedComponent.h
    source/ui/SamplerTab.h
    source/ui/EffectsTab.h
    source/ui/SynthesisTab.h
    source/ui/MasterTab.h)

# Source files - Utilities
target_sources(IndustrialGenerator PRIVATE
    source/utilities/SampleManager.h
    source/utilities/PresetManager.h
    source/utilities/DragDropTarget.h)

# Include directories
target_include_directories(IndustrialGenerator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source)

# Link JUCE modules
target_link_libraries(IndustrialGenerator PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_gui_extra
    juce::juce_dsp)

# Compiler options
if(MSVC)
    target_compile_options(IndustrialGenerator PRIVATE /W4)
else()
    target_compile_options(IndustrialGenerator PRIVATE -Wall -Wextra -Wpedantic)
endif()

# C++17 standard
target_compile_features(IndustrialGenerator PRIVATE cxx_std_17)
