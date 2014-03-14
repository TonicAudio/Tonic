#ifndef __Tonic__ControlRampedValue__
#define __Tonic__ControlRampedValue__

#include "ControlGenerator.h"
#include "ControlTrigger.h"

namespace Tonic {

	namespace Tonic_ {

		class ControlRampedValue_ : public ControlGenerator_{

			protected:

				bool finished_;
				unsigned long count_;
				unsigned long len_;
				TonicFloat target_;
				TonicFloat last_;
				TonicFloat inc_;
				ControlGenerator targetGen_;
				ControlGenerator lengthGen_;
				ControlGenerator valueGen_;
				ControlTrigger finishedTrigger_;
				ControlTrigger shouldLoop_;
				double startTime_;
				void computeOutput(const SynthesisContext_ & context);
				void resetStartTime(double time);

			public:

				ControlRampedValue_();

				void setValueGen( ControlGenerator value );
				void setTargetGen( ControlGenerator target );
				void setLengthGen( ControlGenerator length );
				void setShouldLoopGen(ControlGenerator sholdLoop);

				// internal state setters
				void updateValue( TonicFloat value);
				void updateTarget( TonicFloat target, unsigned long lengthSamp );
				bool isFinished();
				ControlTrigger getFinishedTrigger();

			};

			inline void ControlRampedValue_::computeOutput(const SynthesisContext_ & context){

				bool shouldLoop = shouldLoop_.tick(context).value;

				// First set the value, if necessary (abort ramp, go immediately to value)
				ControlGeneratorOutput valueOutput = valueGen_.tick(context);
				if(valueOutput.triggered){
					updateValue(valueOutput.value);
					resetStartTime(context.elapsedTime);
					finished_ = false;
				}

				// Then update the target or ramp length (start a new ramp)
				ControlGeneratorOutput lengthOutput = lengthGen_.tick(context);
				ControlGeneratorOutput targetOutput = targetGen_.tick(context);
				if (lengthOutput.triggered || targetOutput.triggered){
					finished_ = false;
					unsigned long lSamp = lengthOutput.value*Tonic::sampleRate();
					updateTarget(targetOutput.value, lSamp);
					resetStartTime(context.elapsedTime);
				}

				if (context.elapsedTime - startTime_ >= lengthOutput.value)
				{
					if (!finished_)
					{
						finished_ = true;
						finishedTrigger_.trigger();
					}
				}
				

				if (finished_){
					output_.value = target_;
					output_.triggered = false;
					if (shouldLoop)
					{
						last_ = valueOutput.value;
						finished_ = false;
					}

				}else{
					last_ += inc_;
					if (inc_ > 0)
					{
						if (last_ >= target_)
						{
							last_ = target_;
						}
						
					}else if(inc_ < 0){
						if (last_ <= target_)
						{
							last_ = target_;
						}
					}
					output_.value = last_;
					output_.triggered = true;
					
				}

			}


#pragma mark - Generator setters

			inline void ControlRampedValue_::setValueGen(ControlGenerator value){
				valueGen_ = value;
			}

			inline void ControlRampedValue_::setTargetGen(ControlGenerator target){
				targetGen_ = target;
			}

			inline void ControlRampedValue_::setLengthGen(ControlGenerator length){
				lengthGen_ = length;
			}

			inline void ControlRampedValue_::setShouldLoopGen(ControlGenerator shouldLoop){
				shouldLoop = shouldLoop;
			}

#pragma mark - Internal State setters

			inline void ControlRampedValue_::updateValue( TonicFloat value){

				// When the value gen changes, immediately abort the current ramp and go to the new value.

				finished_ = true;
				last_ = value;
				target_ = value;
				inc_ = 0;
				count_ = 0;
			}

			inline void ControlRampedValue_::updateTarget(TonicFloat target, unsigned long lengthSamp ){

				target_ = target;
				count_ = 0;

				// ND- Fixes bug with NaN when using Accelerate ramps
				if (lengthSamp == 0){
					last_ = target_;
					finished_ = true;
					inc_ = 0;
				}
				else{
					len_ = lengthSamp;
					inc_ = kSynthesisBlockSize * (TonicFloat)(target_ - last_)/len_;
					finished_ = false;
				}

#ifdef TONIC_DEBUG
				if(inc_ != inc_){
					Tonic::error("RampedValue_::updateTarget NaN found\n");
				}
#endif


			}


		}

	

	class ControlRampedValue : public TemplatedControlGenerator<Tonic_::ControlRampedValue_>{

	public:

		ControlRampedValue(TonicFloat startValue = 0, TonicFloat initLength = 0.05){
			target(startValue);
			value(startValue);
			length(initLength);
		}
		    
		TONIC_MAKE_CTRL_GEN_SETTERS(ControlRampedValue, target, setTargetGen);
    
		//! Set length before reaching target value, in seconds
		/*!
			Changes to length gen input will create a new ramp from current value to target over the provided length
		*/
		TONIC_MAKE_CTRL_GEN_SETTERS(ControlRampedValue, length, setLengthGen);
    
		//! Go to value immediately
		/*!
			Changes to the value gen input will abort the current ramp and go immediately to the specified value.
			Output will remain steady until a new target or length is set.
		*/
		TONIC_MAKE_CTRL_GEN_SETTERS(ControlRampedValue, value, setValueGen);

		//! Loop the ramp
		/*!
			If this is true at the moment the ramp finishes, it will reset itself to whatever the value of the "value" generator is.
		*/
		TONIC_MAKE_CTRL_GEN_SETTERS(ControlRampedValue, shouldLoop, setShouldLoopGen);

		//! Get a ControlTrigger which will be triggered when the target value is reached.
		ControlTrigger getFinishedTrigger();

		bool isFinished();

	};

}


#endif