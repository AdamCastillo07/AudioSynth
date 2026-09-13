#pragma once

class Oscillator
{
public:
	enum class Waveform
	{
		Sine,
		Square,
		Saw,
		Triangle
	};

	explicit Oscillator(double sampleRate);

	void setFrequency(double frequency);
	void setSampleRate(double sampleRate);
	void setWaveForm(Waveform waveform);

	float nextSample();

private:
	double sampleRate_;
	double frequency_ = 440.0;
	double phase_ = 0.0;

	Waveform waveform_ = Waveform::Sine;
};