#ifndef __Tonic__ControlMapper__
#define __Tonic__ControlMapper__

#include "ControlConditioner.h"

namespace Tonic{

	namespace Tonic_{

		class ControlMapper_ : public ControlConditioner_{

			typedef struct{
				float x;
				float y;
			} ControlMapperPoint;

			vector<ControlMapperPoint> points_;

		public:
			void computeOutput(const SynthesisContext_ & context);
			void setValues(vector<TonicFloat>);
		};

	}


	/*!
		Maps inputs to outputs, using linear interpolation
	*/

	class ControlMapper : public TemplatedControlConditioner<ControlMapper, Tonic_::ControlMapper_>
	{
	public:
		ControlMapper(void);
		virtual ~ControlMapper(void);

		/*! 
			Accepts a vector in which the values at the even indices are inputs, and the values at the odd indices are outputs
			{0,0, 1,1} would have no effect.
			{0,0, 1,2} would double the input. 
		*/

		ControlMapper& setValues(vector<TonicFloat>);

		/*!
			You can also pass a string of the format "0,0, 1,0.5". C++11 initializer lists would be better, but support is spotty 
		*/
		ControlMapper& setValues(string);
	};

}


#endif

