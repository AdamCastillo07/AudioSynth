#include "AudioEngine.h"
#include <juce_audio_basics/juce_audio_basics.h>

AudioEngine::AudioEngine()
	: oscillator_(44100.0),
	envelope_(44100.0)
{
	oscillator_.setFrequency(261.625565);

	oscillator_.setWaveForm(
		Oscillator::Waveform::Sine
	);

	envelope_.setAttackTime(0.05);
	envelope_.setDecayTime(0.15);
	envelope_.setSustainLevel(0.7f);
	envelope_.setReleaseTime(0.2);

}

void AudioEngine::audioDeviceAboutToStart(
	juce::AudioIODevice* device
)
{
	if (device != nullptr)
	{
		const double sampleRate =
			device->getCurrentSampleRate();

		oscillator_.setSampleRate(sampleRate);
		envelope_.setSampleRate(sampleRate);
	}
}

void AudioEngine::audioDeviceIOCallbackWithContext(
	const float* const* inputChannelData,
	int numInputChannels,
	float* const* outputChannelData,
	int numOutputChannels,
	int numSamples,
	const juce::AudioIODeviceCallbackContext& context
)
{
	juce::ignoreUnused(
		inputChannelData,
		numInputChannels,
		context
	);

	const int requestedNote =
		requestedMidiNote_.load();

	if (requestedNote != activeMidiNote_)
	{
		if (requestedNote < 0)
		{
			envelope_.noteOff();
		}
		else
		{
			const double frequency =
				juce::MidiMessage::getMidiNoteInHertz(
					requestedNote
				);

			oscillator_.setFrequency(frequency);
			envelope_.noteOn();
		} 


		activeMidiNote_ = requestedNote;
	}

	else
	{
		activeVelocity_ =
			requestedVelocity_.load();

		const double frequency =
			juce::MidiMessage::getMidiNoteInHertz(
				requestedNote
			);

		oscillator_.setFrequency(frequency);
		envelope_.noteOn();
	}

	const float velocityGain =
		static_cast<float>(activeVelocity_) / 127.0f;

	for (int sampleIndex = 0;
		sampleIndex < numSamples;
		++sampleIndex)
	{
		const float sample =
			0.1f
			* velocityGain
			* envelope_.nextValue()
			* oscillator_.nextSample();

		for (int channel = 0;
			channel < numOutputChannels;
			++channel)
		{
			if (outputChannelData[channel] != nullptr)
			{
				outputChannelData[channel][sampleIndex] =
					sample;
			}
		}
	}
}
//
//--Notes On/Off
//
void AudioEngine::noteOn(
	int midiNoteNumber,
	int velocity
)
{
	if (midiNoteNumber >= 0
		&& midiNoteNumber <= 127
		&& velocity >= 1
		&& velocity <= 127)
	{
		requestedVelocity_.store(velocity);
		requestedMidiNote_.store(midiNoteNumber);
	}
}

void AudioEngine::handleIncomingMidiMessage(
	juce::MidiInput* source,
	const juce::MidiMessage& message
)
{
	juce::ignoreUnused(source);

	if (message.isNoteOn())
	{
		noteOn(
			message.getNoteNumber(),
			message.getVelocity()
		);;
	}
	else if (message.isNoteOff())
	{
		noteOff(message.getNoteNumber());
	}
}

void AudioEngine::noteOff(int midiNoteNumber)
{
	if (requestedMidiNote_.load() == midiNoteNumber)
	{
		requestedMidiNote_.store(-1);
	}
}

void AudioEngine::audioDeviceStopped() {

}