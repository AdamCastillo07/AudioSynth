# CMakeList.txt : CMake project for AudioSynth, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (AudioSynth "AudioSynth.cpp" "AudioSynth.h"
"Oscillator.cpp" "Oscillator.h" "AmplitudeEnvelope.h" "AmplitudeEnvelope.cpp" "WavWriter.h" "WavWriter.cpp" "AudioEngine.h" "AudioEngine.cpp")

target_link_libraries(AudioSynth
    PRIVATE
        juce::juce_core
        juce::juce_events
        juce::juce_audio_basics
        juce::juce_audio_devices
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AudioSynth PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
