#pragma once

#include "AmplitudeEnvelope.h"
#include "Oscillator.h"

class SynthVoice
{
public:
    explicit SynthVoice(double sampleRate);

    void setSampleRate(double sampleRate);
    void setWaveForm(Oscillator::Waveform waveform);

    void startNote(int midiNoteNumber, int velocity);
    void stopNote();

    float nextSample();

    bool isActive() const;
    int getMidiNoteNumber() const;

private:
    Oscillator oscillator_;
    AmplitudeEnvelope envelope_;

    int midiNoteNumber_ = -1;
    float velocityGain_ = 0.0f;
};