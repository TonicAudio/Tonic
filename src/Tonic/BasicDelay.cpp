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
    delayTimeGen_ = FixedValue(0);
    fbkGen_ = FixedValue(0);
    setDryLevelGen(FixedValue(0.5));
    setWetLevelGen(FixedValue(0.5));
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
    if (maxDelayTime <= 0) maxDelayTime = delayTime * 1.5;
    delayLine_.initialize(maxDelayTime, 1);
    delayTimeGen_ = FixedValue(delayTime);
  }
  
} // Namespace Tonic_
  
  BasicDelay::BasicDelay(float initialDelayTime, float maxDelayTime){
    gen()->initialize(initialDelayTime, maxDelayTime);
    delayTime(initialDelayTime);
  }
  
  
} // Namespace Tonic
