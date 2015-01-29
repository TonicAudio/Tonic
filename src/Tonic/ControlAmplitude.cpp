//
//  ControlAmplitude.cpp
//  Tonic
//
//

#include "ControlAmplitude.h"

namespace Tonic {
	namespace Tonic_{

		void ControlAmplitude_::computeOutput(const SynthesisContext_ &context)
		{
			input_.tick(inputFrames, context);
			TonicFloat total = 0;
			TonicFloat* data = &inputFrames[0];
			for (int i = 0; i < kSynthesisBlockSize; i++)
			{
				total +=  fabs( *data );
				data++;
			}
			output_.value = total / kSynthesisBlockSize;
		}

		ControlAmplitude_::ControlAmplitude_()
		{
			inputFrames.resize(kSynthesisBlockSize, 1);
		}

		ControlAmplitude_::~ControlAmplitude_()
		{

		}



	}


} // Namespace Tonic
