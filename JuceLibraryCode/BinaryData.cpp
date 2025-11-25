/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#include <cstring>

namespace BinaryData
{

//================== CMakeLists.txt ==================
static const unsigned char temp_binary_data_0[] =
"cmake_minimum_required(VERSION 3.15)\n"
"\n"
"project(BM11MLPF)\n"
"\n"
"# JUCE specific configuration\n"
"find_package(JUCE REQUIRED CONFIG)\n"
"\n"
"juce_add_plugin(BM11MLPF\n"
"    # VERSION ...               # Set this to the plugin version\n"
"    # ICON_BIG ...              # Set this to the path of your big icon\n"
"    # ICON_SMALL ...            # Set this to the path of your small icon\n"
"    COMPANY_NAME \"YourCompany\"  # Set this to your company name\n"
"    IS_SYNTH FALSE              # Is a synth?\n"
"    NEEDS_MIDI_INPUT FALSE      # Needs midi input?\n"
"    NEEDS_MIDI_OUTPUT FALSE     # Needs midi output?\n"
"    IS_MIDI_EFFECT FALSE        # Is a midi effect?\n"
"    EDITOR_WANTS_KEYBOARD_FOCUS FALSE\n"
")\n"
"\n"
"# Source files\n"
"target_sources(BM11MLPF PRIVATE\n"
"    Source/BM11MLPFAudioProcessor.cpp\n"
"    Source/BM11MLPFAudioProcessorEditor.cpp\n"
")\n"
"\n"
"# Headers\n"
"target_sources(BM11MLPF PRIVATE\n"
"    Source/BM11MLPFAudioProcessor.h\n"
"    Source/BM11MLPFAudioProcessorEditor.h\n"
")\n"
"\n"
"# JUCE modules\n"
"target_link_libraries(BM11MLPF PRIVATE\n"
"    juce::juce_audio_utils\n"
"    juce::juce_dsp\n"
")\n"
"\n"
"# Formatting\n"
"juce_generate_juce_header(BM11MLPF)";

const char* CMakeLists_txt = (const char*) temp_binary_data_0;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes);
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x90e15cf5:  numBytes = 1073; return CMakeLists_txt;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "CMakeLists_txt"
};

const char* originalFilenames[] =
{
    "CMakeLists.txt"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
        if (strcmp (namedResourceList[i], resourceNameUTF8) == 0)
            return originalFilenames[i];

    return nullptr;
}

}
