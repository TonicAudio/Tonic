//
//  MonoDelay.cpp
//  Tonic
//
//  Created by Nick Donaldson on 3/10/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "MonoDelay.h"

namespace Tonic { namespace Tonic_{
  
  MonoDelay_::MonoDelay_() : delayLine_(NULL) {
    delayTimeFrames_.resize(kSynthesisBlockSize, 1, 0);
    fbkFrames_.resize(kSynthesisBlockSize, 1, 0);
    mixFrames_.resize(kSynthesisBlockSize, 1, 0);
  }
  
  MonoDelay_::~MonoDelay_(){
    if (delayLine_) delete delayLine_;
  }
  
  void MonoDelay_::initialize(float delayTime, float maxDelayTime)
  {
    if (delayLine_ == NULL){
      
      if (maxDelayTime < 0) maxDelayTime = delayTime * 1.5;
      
      if (maxDelayTime < delayTime){
        error("MonoDelay_ - initial delay time is greater than max delay time", true);
      }
      else if (delayTime <= 0){
        error("MonoDelay_ - initial delay time must be greater than zero");
      }
      
      delayLine_ = new DelayLine(delayTime, maxDelayTime, 1);
    }
    else{
      warning("MonoDelay_::initialize has already been called.");
    }
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
