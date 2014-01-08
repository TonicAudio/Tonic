#include "ControlSineOsc.h"

Tonic::Tonic_::ControlSineOsc_::ControlSineOsc_() : phase_(0)
{
}

void Tonic::Tonic_::ControlSineOsc_::computeOutput( const SynthesisContext_ & context )
{
	float freq = freq_.tick(context).value;
	float cyclesPerFrame = freq / Tonic::sampleRate();
	float phaseInc = cyclesPerFrame * kSynthesisBlockSize;
	phase_ = fmod(phase_ + phaseInc,  TWO_PI );
	output_.value = sin(phase_);
	output_.triggered = false;
}


