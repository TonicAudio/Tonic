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

			struct Point{float value; float time;};

			ControlRampedValue		rampedValue_;
			vector<Point>			points_;
			int						currentPoint_;
			double					timeOfLastBreakpointStart;
			bool					finished_;

			inline void computeOutput(const SynthesisContext_ & context){
				output_ = rampedValue_.tick(context);

				// maybe remove this check and initialize the time with a start() method
				if (timeOfLastBreakpointStart < 0)
				{
					timeOfLastBreakpointStart = context.elapsedTime;
				}
				
				if ( context.elapsedTime - timeOfLastBreakpointStart  >=  points_.at(currentPoint_).time )
				{
					timeOfLastBreakpointStart = context.elapsedTime;

					if(currentPoint_ < points_.size() -1){
						goToPoint(currentPoint_ + 1);
					}else{
						finished_ = true;
					}
				}
			}

			public:

				ControlBreakpoints_();
				void setPoints(vector<float> points);
				void goToPoint(int point);
				bool isFinished();
		};

	}

	class ControlBreakpoints : public TemplatedControlGenerator<Tonic_::ControlBreakpoints_> {
	public:
		ControlBreakpoints&	points(vector<float> points);
		ControlBreakpoints&	goToPoint(int point);
		bool			isFinished();
	};

}

#endif /* defined(__TonicDemo__ControlBreakpoints__) */
