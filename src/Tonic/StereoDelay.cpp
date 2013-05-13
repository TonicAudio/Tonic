//
//  StereoDelay.cpp
//  Tonic
//
//  Created by Morgan Packard on 3/13/13.

//

#include "StereoDelay.h"

namespace Tonic { namespace Tonic_{
  
  StereoDelay_::StereoDelay_(){
    setIsStereoOutput(true);
    setIsStereoInput(true);
    delayTimeFrames_[0].resize(kSynthesisBlockSize, 1, 0);
    delayTimeFrames_[1].resize(kSynthesisBlockSize, 1, 0);
    fbkFrames_.resize(kSynthesisBlockSize, 1, 0);
    setDryLevelGen(FixedValue(0.5));
    setWetLevelGen(FixedValue(0.5));
  }
  
  void StereoDelay_::initialize(float leftDelayArg, float rightDelayArg, float maxDelayLeft, float maxDelayRight){

    delayLine_[0].initialize(leftDelayArg, maxDelayLeft);
    delayLine_[1].initialize(rightDelayArg, maxDelayRight);
  }

  
} // Namespace Tonic_
  
  StereoDelay:: StereoDelay(float leftDelay, float rightDelay, float maxDelayLeft, float maxDelayRight){
    gen()->initialize(leftDelay, rightDelay, maxDelayLeft, maxDelayRight);
    delayTimeLeft(leftDelay);
    delayTimeRight(rightDelay);
  }
  
} // Namespace Tonic
