//
//  ControlTimingUtils.cpp
//  Tonic
//
//  Created by Nick Donaldson on 3/10/13.


#include "ControlTimingUtils.h"

namespace Tonic {
  
  namespace Tonic_{
    
    ControlMetro_::ControlMetro_() : lastClickTime_(0) {}
    
    // ----
    
    ControlMetroDivider_::ControlMetroDivider_() : tickCounter_(0) {
      divisonsGen_ = ControlValue(1);
      offsetGen_ = ControlValue(0);
      output_.value = 1.f;
    }
    
    // ----
    
    ControlPulse_::ControlPulse_() : lastOnTime_(0) {}
    
    // ----
    
    ControlDelay_::ControlDelay_() :
    readHead_(0),
    writeHead_(0),
    maxDelay_(0)
    {}
    
    void ControlDelay_::initialize(float maxDelayTime){
      maxDelay_ = max(maxDelayTime * sampleRate() / kSynthesisBlockSize, 1);
      delayLine_.resize(maxDelay_);
    }
  
  
  } // Namespace Tonic_
  
} // Namespace Tonic
