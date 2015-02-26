//
//  Generator.cpp
//  Tonic
//
//  Created by Morgan Packard on 1/23/13.
//
//
// See LICENSE.txt for license and usage information.
//


#include "Generator.h"
#include "MonoToStereoPanner.h"
#include "ControlGenerator.h"

namespace Tonic{ 

namespace Tonic_{
  
	Generator_::Generator_() : isStereoOutput_(false), lastFrameIndex_(0) {
		outputFrames_.resize(kSynthesisBlockSize, 1, 0);
	}

	Generator_::~Generator_() {}

	void Generator_::setIsStereoOutput(bool stereo){
		if (stereo != isStereoOutput_){
			outputFrames_.resize(kSynthesisBlockSize, stereo ? 2 : 1, 0);
		}
		isStereoOutput_ = stereo;
	}

}


	Generator Generator::pan(ControlGenerator pan)
	{
		if (isStereoOutput())
		{
			error("Currently, Generator::pan only works on mono signals.");
		}
		
		return MonoToStereoPanner().input(*this).pan(pan);
	}

	Generator Generator::pan(float panVal)
	{
		return pan(ControlValue().value(panVal));
	}


}
