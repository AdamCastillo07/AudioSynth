#include "AmplitudeEnvelope.h"

AmplitudeEnvelope::AmplitudeEnvelope(double sampleRate)
	: sampleRate_(sampleRate)
{
}
//
//--Setters
//
void AmplitudeEnvelope::setSampleRate(double sampleRate)
{
	if (sampleRate > 0.0)
	{
		sampleRate_ = sampleRate;
	}
}

void AmplitudeEnvelope::setAttackTime(double seconds)
{
	if (seconds < 0.0)
	{
		seconds = 0.0;
	}

	attackTime_ = seconds;
}

void AmplitudeEnvelope::setDecayTime(double seconds)
{
	if (seconds < 0.0)
	{
		seconds = 0.0;
	}
}

void AmplitudeEnvelope::setSustainLevel(float level)
{
	if (level < 0.0f)
	{
		level = 0.0f;
	}
	else if (level > 1.0f)
	{
		level = 1.0f;
	}

	sustainLevel_ = level;
}

void AmplitudeEnvelope::setReleaseTime(double seconds)
{
	if (seconds < 0.0)
	{
		seconds = 0.0;
	}

	releaseTime_ = seconds;
}

void AmplitudeEnvelope::noteOn()
{
	state_ = State::Attack;
}

void AmplitudeEnvelope::noteOff()
{
	if (state_ == State::Idle) {
		return;
	}
	

	state_ = State::Release;

	const double releaseSamples =
		releaseTime_ * sampleRate_;

	if (releaseSamples > 0.0)
	{
		releaseStep_ = static_cast<float>(
			currentLevel_ / releaseSamples
			);
	}

	else
	{
		releaseStep_ = currentLevel_;
	}
}

float AmplitudeEnvelope::nextValue()
{
	switch (state_)
	{
	case State::Idle:
		currentLevel_ = 0.0f;
		break;

	case State::Attack:
		if (attackTime_ <= 0.0)
		{
			currentLevel_ = 1.0f;
			state_ = State::Decay;
		}
		else
		{
			const double attackSamples =
				attackTime_ * sampleRate_;

			currentLevel_ += static_cast<float>(
				1.0 / attackSamples
				);

			if (currentLevel_ >= 1.0f)
			{
				currentLevel_ = 1.0f;
				state_ = State::Decay;
			}
		}
		break;

	case State::Decay:
		if (decayTime_ <= 0.0)
		{
			currentLevel_ = sustainLevel_;
			state_ = State::Sustain;
		}
		else
		{
			const double decaySamples =
				decayTime_ * sampleRate_;

			const double decayStep =
				(1.0 - sustainLevel_) / decaySamples;

			currentLevel_ -= static_cast<float>(decayStep);

			if (currentLevel_ <= sustainLevel_)
			{
				currentLevel_ = sustainLevel_;
				state_ = State::Sustain;
			}
		}
		break;

	case State::Sustain:
		currentLevel_ = sustainLevel_;
		break;

	case State::Release:
		currentLevel_ -= releaseStep_;

		if (currentLevel_ <= 0.0f)
		{
			currentLevel_ = 0.0f;
			state_ = State::Idle;
		}
		break;
	}

	return currentLevel_;
}