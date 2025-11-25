cmake_minimum_required(VERSION 3.15)

project(BM11MLPF)

# JUCE specific configuration
find_package(JUCE REQUIRED CONFIG)

juce_add_plugin(BM11MLPF
    # VERSION ...               # Set this to the plugin version
    # ICON_BIG ...              # Set this to the path of your big icon
    # ICON_SMALL ...            # Set this to the path of your small icon
    COMPANY_NAME "YourCompany"  # Set this to your company name
    IS_SYNTH FALSE              # Is a synth?
    NEEDS_MIDI_INPUT FALSE      # Needs midi input?
    NEEDS_MIDI_OUTPUT FALSE     # Needs midi output?
    IS_MIDI_EFFECT FALSE        # Is a midi effect?
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
)

# Source files
target_sources(BM11MLPF PRIVATE
    Source/BM11MLPFAudioProcessor.cpp
    Source/BM11MLPFAudioProcessorEditor.cpp
)

# Headers
target_sources(BM11MLPF PRIVATE
    Source/BM11MLPFAudioProcessor.h
    Source/BM11MLPFAudioProcessorEditor.h
)

# JUCE modules
target_link_libraries(BM11MLPF PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
)

# Formatting
juce_generate_juce_header(BM11MLPF)