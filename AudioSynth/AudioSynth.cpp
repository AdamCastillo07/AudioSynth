//--AudioSynth.cpp : Defines the entry point for the application.
//
#include "AmplitudeEnvelope.h"
#include "Oscillator.h"
#include "WavWriter.h"
#include "AudioEngine.h"
//
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_events/juce_events.h>
//
#include <limits>
#include <algorithm>
#include <cstdint>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include <filesystem>
#include <string>
//
//--write ints in the byte order required by WAV files
//

//
//
//--Main function
//
//
int main() {
	juce::ScopedJuceInitialiser_GUI juceInitialiser;
 
	constexpr int sampleRate = 44100;
	
	constexpr double firstFrequency = 261.625565;  // Middle C
	constexpr double secondFrequency = 391.995436; // G above middle C

	constexpr double attackTime = 0.01;
	constexpr double decayTime = 0.5;
	constexpr float sustainLevel = 0.2f;
	constexpr double releaseTime = 0.1;

	constexpr double amplitude = 0.25;
	constexpr int durationSeconds = 2;

	const int totalSamples = sampleRate * durationSeconds;
	const int releaseSamples = static_cast<int>(
		sampleRate * releaseTime
		);


	std::vector<float> samples(totalSamples);
	//
	//--Oscillator generation
	//
	Oscillator oscillator(sampleRate);
	oscillator.setFrequency(firstFrequency);
	//
	//--Envelope
	// 
	AmplitudeEnvelope envelope(sampleRate);

	envelope.setAttackTime(attackTime);
	envelope.setDecayTime(decayTime);
	envelope.setSustainLevel(sustainLevel);
	envelope.setReleaseTime(releaseTime);
	envelope.noteOn();
	// 
	//--User oscillator choice
	//
	std::cout
		<< "Choose Waveform:\n"
		<< "1. Sine\n"
		<< "2. Square\n"
		<< "3. Saw\n"
		<< "4. Triangle\n"
		<< "Enter your choice: ";

	int waveFormChoice = 0;

	if (!(std::cin >> waveFormChoice))
	{
		std::cerr << "Please enter a number.\n";
		return 1;
	}  

	switch (waveFormChoice)
	{
	case 1:
		oscillator.setWaveForm(Oscillator::Waveform::Sine);
		break;
	case 2:
		oscillator.setWaveForm(Oscillator::Waveform::Square);
		break;
	case 3:
		oscillator.setWaveForm(Oscillator::Waveform::Saw);
		break;
	case 4:
		oscillator.setWaveForm(Oscillator::Waveform::Triangle);
		break;
	default:
		std::cerr << "Invalid waveform choice.\n";
		return 1;
	}

	std::string userFileName = "";
	
	std::cout
		<< "Enter a name for your .wav file:\n";
	std::cin >> userFileName;
	
	//
	//--Generation loop
	//
	for (int sampleIndex = 0;
		sampleIndex < totalSamples;
		++sampleIndex)
	{
		if (sampleIndex == sampleRate - releaseSamples)
		{
			envelope.noteOff();
		}

		if (sampleIndex == sampleRate)
		{
			oscillator.setFrequency(secondFrequency);
			envelope.noteOn();
		}

		if (sampleIndex == totalSamples - releaseSamples)
		{
			envelope.noteOff();
		}

		const float envelopeValue =
			envelope.nextValue();

		samples[sampleIndex] = static_cast<float>(
			amplitude
			* envelopeValue
			* oscillator.nextSample()
			);
	}

	std::vector<std::int16_t> pcmSamples(totalSamples);

	for (int sampleIndex = 0; sampleIndex < totalSamples; ++sampleIndex) {
		const float clampedSample =
			std::clamp(samples[sampleIndex], -1.0f, 1.0f);

		pcmSamples[sampleIndex] = static_cast<std::int16_t>(
			std::round(clampedSample * 32767.0f)
		);
	}

	const std::string filename = userFileName + ".wav";

	if (!writeWavFile(filename, pcmSamples, sampleRate))
	{
		std::cerr << "Failed to write the WAV file.\n";
		return 1;
	}

	std::cout
		<< "Wrote WAV file to:\n"
		<< std::filesystem::absolute(filename)
		<< '\n';

	std::cout << "Generated " << samples.size() << " samples.\n";
	std::cout << "First 10 samples:\n";

	std::cout << std::fixed << std::setprecision(6);

	for (int sampleIndex = 0; sampleIndex < 10; ++sampleIndex)
	{
		std::cout
			<< sampleIndex
			<< ": float = "
			<< samples[sampleIndex]
			<< ", PCM = "
			<< pcmSamples[sampleIndex]
			<< '\n';
	}
	//--Live audio test

	AudioEngine audioEngine;
	juce::AudioDeviceManager deviceManager;

	const juce::String audioError =
		deviceManager.initialise(
			0,
			2,
			nullptr,
			true
		);

	if (audioError.isNotEmpty())
	{
		std::cerr
			<< "Audio device error: "
			<< audioError.toStdString()
			<< '\n';

		return 1;
	}

	//deviceManager.addAudioCallback(&audioEngine);

	const auto midiDevices =
		juce::MidiInput::getAvailableDevices();

	if (midiDevices.isEmpty())
	{
		std::cerr << "No MIDI input devices found.\n";
		return 1;
	}

	std::cout << "Available MIDI input devices:\n";

	for (int index = 0; index < midiDevices.size(); ++index)
	{
		std::cout
			<< index + 1
			<< ". "
			<< midiDevices[index].name.toStdString()
			<< '\n';
	}

	std::cout << "Choose a MIDI device: ";

	int midiDeviceChoice = 0;
	std::cin >> midiDeviceChoice;

	if (midiDeviceChoice < 1
		|| midiDeviceChoice > midiDevices.size())
	{
		std::cerr << "Invalid MIDI device choice.\n";
		return 1;
	}

	const auto selectedMidiDevice =
		midiDevices[midiDeviceChoice - 1];

	deviceManager.setMidiInputDeviceEnabled(
		selectedMidiDevice.identifier,
		true
	);

	deviceManager.addMidiInputDeviceCallback(
		selectedMidiDevice.identifier,
		&audioEngine
	);

	deviceManager.addAudioCallback(&audioEngine);
	

	std::cout
		<< "Play your MIDI controller.\n"
		<< "Press Enter to stop.\n";

	std::cin.ignore(
		std::numeric_limits<std::streamsize>::max(),
		'\n'
	);

	std::cin.get();

	deviceManager.removeMidiInputDeviceCallback(
		selectedMidiDevice.identifier,
		&audioEngine
	);

	deviceManager.removeAudioCallback(&audioEngine);
	deviceManager.closeAudioDevice();


	return 0;
}

