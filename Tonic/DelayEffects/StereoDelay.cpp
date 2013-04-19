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
    mixFrames_.resize(kSynthesisBlockSize, 1, 0);
  }
  
  void StereoDelay_::initialize(float leftDelayArg, float rightDelayArg, float maxDelayLeft, float maxDelayRight){
    
    if (maxDelayLeft < 0) maxDelayLeft = leftDelayArg * 1.5;
    if (maxDelayRight < 0) maxDelayRight = rightDelayArg * 1.5;
    
    if (maxDelayLeft < leftDelayArg || maxDelayRight < rightDelayArg){
      error("StereoDelay_ - initial delay time is greater than max delay time", true);
    }
    else if (leftDelayArg <= 0 || rightDelayArg <= 0){
      error("StereoDelay_ - initial delay time must be greater than zero");
    }
    
    delayLine_[0].initialize(leftDelayArg, maxDelayLeft);
    delayLine_[1].initialize(rightDelayArg, maxDelayRight);
  }
  
  StereoDelay_::~StereoDelay_(){
    
  }

  
} // Namespace Tonic_
  
  StereoDelay:: StereoDelay(float leftDelay, float rightDelay, float maxDelayLeft, float maxDelayRight){
    gen()->initialize(leftDelay, rightDelay, maxDelayLeft, maxDelayRight);
  }
  
} // Namespace Tonic
