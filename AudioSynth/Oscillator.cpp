#include "Oscillator.h"

#include <cmath>

Oscillator::Oscillator(double sampleRate)
	: sampleRate_(sampleRate)
{
}

void Oscillator::setFrequency(double frequency)
{
	frequency_ = frequency;
}

void Oscillator::setSampleRate(double sampleRate)
{
	if (sampleRate > 0.0)
	{
		sampleRate_ = sampleRate;
	}
}

void Oscillator::setWaveForm(Oscillator::Waveform waveform)
{
	waveform_ = waveform;
}

float Oscillator::nextSample()
{
	constexpr double twoPi = 6.283185307179586;

	float sample = 0.0f;

	switch(waveform_)
	{
	case Waveform::Sine: //
		sample = static_cast<float>(
			std::sin(twoPi * phase_)
		);
		break;

	case Waveform::Square: //
		sample = phase_ < 0.5 ? 1.0f : -1.0f;
		break;

	case Waveform::Saw: //
		sample = static_cast<float>(
			2.0 * phase_ - 1.0
			);
		break;
	case Waveform::Triangle: //
		sample = static_cast<float>(
			1.0 - 4.0 * std::abs(phase_ - 0.5)
			);
		break;
	}



	phase_ += frequency_ / sampleRate_;

	if (phase_ >= 1.0)
	{
		phase_ -= 1.0;
	}

	return sample;
}

