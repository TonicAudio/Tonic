
#include "ControlRampedValue.h"

Tonic::Tonic_::ControlRampedValue_::ControlRampedValue_() : 	
	finished_(false),
	count_(0),
	len_(0),
	target_(0),
	last_(0),
	inc_(0)
{

}

Tonic::ControlTrigger Tonic::Tonic_::ControlRampedValue_::getFinishedTrigger()
{
	return finishedTrigger_;
}

bool Tonic::Tonic_::ControlRampedValue_::isFinished()
{
	return finished_;
}

void Tonic::Tonic_::ControlRampedValue_::resetStartTime( double time )
{
	startTime_ = time;
}

Tonic::ControlTrigger Tonic::ControlRampedValue::getFinishedTrigger()
{
	return gen()->getFinishedTrigger();
}

bool Tonic::ControlRampedValue::isFinished()
{
	return gen()->isFinished();
}
