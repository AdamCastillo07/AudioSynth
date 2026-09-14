#pragma once

#include "Oscillator.h"
#include "AmplitudeEnvelope.h"
#include <atomic>

#include <juce_audio_devices/juce_audio_devices.h>

class AudioEngine final : public juce::AudioIODeviceCallback,
    public juce::MidiInputCallback
{
public:
    AudioEngine();
    

    void audioDeviceAboutToStart(
        juce::AudioIODevice* device
    ) override;

    void audioDeviceIOCallbackWithContext(
        const float* const* inputChannelData,
        int numInputChannels,
        float* const* outputChannelData,
        int numOutputChannels,
        int numSamples,
        const juce::AudioIODeviceCallbackContext& context
    ) override;

    void handleIncomingMidiMessage(
        juce::MidiInput* source,
        const juce::MidiMessage& message
    ) override;

    void noteOn(int midiNoteNumber, int velocity);
    void noteOff(int midiNoteNumber);

    void audioDeviceStopped() override;

    

private:
    Oscillator oscillator_;
    AmplitudeEnvelope envelope_;
   
    std::atomic<int> requestedMidiNote_{ -1 };
    int activeMidiNote_ = -1;

    std::atomic<int> requestedVelocity_{ 0 };
    int activeVelocity_ = 0;
};