#include "ControlBreakpoints.h"

namespace Tonic{

	namespace Tonic_{

		ControlBreakpoints_::ControlBreakpoints_() : currentPoint_(0), timeOfLastBreakpointStart(-1), finished_(false)
		{

		}

		void ControlBreakpoints_::setPoints( vector<float> points )
		{
			points_.clear();
			for(int i = 0; i < points.size(); i += 2){
				Point point;
				point.time = points.at(i);
				point.value = points.at(i +1);
				points_.push_back(point);
			}
			goToPoint(currentPoint_);
		}

		void ControlBreakpoints_::goToPoint( int point )
		{
			currentPoint_ = clamp(point, 0, points_.size() - 1);
			Point current = points_.at(currentPoint_);
			float duration = current.time;
			if (currentPoint_ > 0)
			{
				duration -= points_.at(currentPoint_ - 1).time;
			}
			rampedValue_.target(current.value).length(duration);
		}

		bool ControlBreakpoints_::isFinished()
		{
			return finished_;
		}

	}

	ControlBreakpoints& Tonic::ControlBreakpoints::points( vector<float> points )
	{
		gen()->setPoints(points);
		return *this;
	}


	ControlBreakpoints& ControlBreakpoints::goToPoint( int point )
	{
		gen()->goToPoint(point);
		return *this;
	}

	bool ControlBreakpoints::isFinished()
	{
		return isFinished();
	}

}
