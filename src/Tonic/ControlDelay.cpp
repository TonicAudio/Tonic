//
//  ControlDelay.cpp
//  Tonic
//
//  Created by Nick Donaldson on 5/5/13.
//

#include "ControlDelay.h"

namespace Tonic {
  
  namespace Tonic_{
  
    ControlDelay_::ControlDelay_() :
      readHead_(0),
      writeHead_(0),
      maxDelay_(0)
    {
    }

    void ControlDelay_::initialize(float maxDelayTime){
      maxDelay_ = max(maxDelayTime * sampleRate() / kSynthesisBlockSize, 1);
      delayLine_.resize(maxDelay_);
      readHead_ = maxDelay_ - 1;
    }

  } // Namespace Tonic_
  
  ControlDelay::ControlDelay(float maxDelayTime){
    this->gen()->initialize(maxDelayTime);
  }
  
} // Namespace Tonic
