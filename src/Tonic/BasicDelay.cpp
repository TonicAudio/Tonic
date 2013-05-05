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
    delayTimeGen_ = FixedValue(0);
    fbkGen_ = FixedValue(0);
    mixGen_ = FixedValue(0);
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
    delayLine_.initialize(delayTime, maxDelayTime, 1);
  }
  
} // Namespace Tonic_
  
  BasicDelay::BasicDelay(float initialDelayTime, float maxDelayTime){
    gen()->initialize(initialDelayTime, maxDelayTime);
    delayTime(initialDelayTime);
  }
  
  
} // Namespace Tonic