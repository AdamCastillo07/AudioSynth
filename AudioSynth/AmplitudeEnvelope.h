#pragma once

class AmplitudeEnvelope
{
public:
	explicit AmplitudeEnvelope(double sampleRate);
	
	void setSampleRate(double sampleRate);
	void setAttackTime(double seconds);
	void setDecayTime(double seconds);
	void setSustainLevel(float level);
	void setReleaseTime(double seconds);
	
	bool isActive() const;
	
	void noteOn();
	void noteOff();

	float nextValue();

private:
	enum class State
	{
		Idle,
		Attack,
		Decay,
		Sustain,
		Release
	};

	double sampleRate_;
	
	double attackTime_ = 0.01;
	double decayTime_ = 0.1;
	float sustainLevel_ = 0.7f;
	double releaseTime_ = 0.1;
	
	float currentLevel_ = 0.0f;
	float releaseStep_ = 0.0f;

	State state_ = State::Idle;
};