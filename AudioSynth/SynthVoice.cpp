#include "SynthVoice.h"

#include <cmath>

SynthVoice::SynthVoice(double sampleRate)
    : oscillator_(sampleRate),
    envelope_(sampleRate)
{
    oscillator_.setWaveForm(
        Oscillator::Waveform::Sine
    );

    envelope_.setAttackTime(0.05);
    envelope_.setDecayTime(0.15);
    envelope_.setSustainLevel(0.7f);
    envelope_.setReleaseTime(0.2);
}

void SynthVoice::setSampleRate(double sampleRate)
{
    oscillator_.setSampleRate(sampleRate);
    envelope_.setSampleRate(sampleRate);
}

void SynthVoice::setWaveForm(
    Oscillator::Waveform waveform
)
{
    oscillator_.setWaveForm(waveform);
}

void SynthVoice::startNote(
    int midiNoteNumber,
    int velocity
)
{
    if (midiNoteNumber < 0
        || midiNoteNumber > 127
        || velocity < 1
        || velocity > 127)
    {
        return;
    }

    midiNoteNumber_ = midiNoteNumber;
    velocityGain_ =
        static_cast<float>(velocity) / 127.0f;

    const double semitonesFromA =
        (midiNoteNumber - 69) / 12.0;

    const double frequency =
        440.0 * std::pow(2.0, semitonesFromA);

    oscillator_.setFrequency(frequency);
    envelope_.noteOn();
}

void SynthVoice::stopNote()
{
    envelope_.noteOff();
}

float SynthVoice::nextSample()
{
    if (!envelope_.isActive())
    {
        midiNoteNumber_ = -1;
        velocityGain_ = 0.0f;
        return 0.0f;
    }

    const float envelopeValue =
        envelope_.nextValue();

    const float sample =
        velocityGain_
        * envelopeValue
        * oscillator_.nextSample();

    if (!envelope_.isActive())
    {
        midiNoteNumber_ = -1;
        velocityGain_ = 0.0f;
    }

    return sample;
}

bool SynthVoice::isActive() const
{
    return envelope_.isActive();
}

int SynthVoice::getMidiNoteNumber() const
{
    return midiNoteNumber_;
}