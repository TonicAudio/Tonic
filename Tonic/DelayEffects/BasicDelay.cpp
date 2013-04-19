//
//  BasicDelay.cpp
//  Tonic
//
//  Created by Nick Donaldson on 3/10/13.

//

#include "BasicDelay.h"

namespace Tonic { namespace Tonic_{
  
  BasicDelay_::BasicDelay_() {
    delayTimeFrames_.resize(kSynthesisBlockSize, 1, 0);
    fbkFrames_.resize(kSynthesisBlockSize, 1, 0);
    mixFrames_.resize(kSynthesisBlockSize, 1, 0);
  }
  
  BasicDelay_::~BasicDelay_(){
    
  }
  
  void BasicDelay_::setInput(Generator input){
    Effect_::setInput(input);
    setIsStereoInput(input.isStereoOutput());
    setIsStereoOutput(input.isStereoOutput());
    
    // can safely resize as TonicFrames subclass - calling functions account for channel offset
    delayLine_.resize(delayLine_.frames(), input.isStereoOutput() ? 2 : 1, 0);
  }
  
  void BasicDelay_::initialize(float delayTime, float maxDelayTime)
  {
    if (maxDelayTime < 0) maxDelayTime = delayTime * 1.5;
       
    if (maxDelayTime < delayTime){
      error("BasicDelay_ - initial delay time is greater than max delay time", true);
    }
    else if (delayTime <= 0){
      error("BasicDelay_ - initial delay time must be greater than zero");
    }
    
    delayLine_.initialize(delayTime, maxDelayTime, 1);
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
