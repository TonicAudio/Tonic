//  ControlBreakpoints.h
//  TonicDemo
//

//
// See LICENSE.txt for license and usage information.
//

/*

A breakpoint envelope. Takes a vector of duration/value pairs, where the second number is the final value, and the first is the amount of time (in seconds) it takes to get there. 
*/

#ifndef __TonicDemo__ControlBreakpoints__
#define __TonicDemo__ControlBreakpoints__

#include "ControlGenerator.h"
#include "ControlRampedValue.h"

namespace Tonic{

	namespace Tonic_{

		class ControlBreakpoints_ : public ControlGenerator_{

			enum State{
				INITAL,
				RUNNING,
				FINISHED
			};

			struct Point{float value; float time;};

			ControlRampedValue		rampedValue_;
			vector<Point>			points_;
			int						currentPoint_;
			ControlGenerator		trigger_;
			State					state_;
			ControlTrigger			finishedTrigger_;

			inline void computeOutput(const SynthesisContext_ & context){
				output_ = rampedValue_.tick(context);
				ControlGeneratorOutput triggerOut = trigger_.tick(context);

				if (triggerOut.triggered)
				{
					goToPoint(0);
					state_ = RUNNING;
				}
				
				if (rampedValue_.isFinished())
				{
					if (currentPoint_ >= points_.size() -1)
					{
						if (state_ != FINISHED)
						{
							state_ = FINISHED;
							finishedTrigger_.trigger();
						}
						
					}else
					{
						goToPoint(currentPoint_ + 1);
					}
					
				}
				
			}

			public:

				ControlBreakpoints_();
				void setPoints(vector<float> points);
				void goToPoint(int point);
				bool isFinished();
				void setTrigger(ControlGenerator trigger);
				ControlGenerator getFinishedTrigger();
		};

	}

	class ControlBreakpoints : public TemplatedControlGenerator<Tonic_::ControlBreakpoints_> {
	public:
		TONIC_MAKE_CTRL_GEN_SETTERS(ControlBreakpoints, trigger, setTrigger)
		ControlBreakpoints&		points(vector<float> points);
		ControlBreakpoints&		goToPoint(int point);
		bool					isFinished();
		ControlGenerator		getFinishedTrigger();
	};

}

#endif /* defined(__TonicDemo__ControlBreakpoints__) */
