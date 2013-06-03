//
//  ControlPulse.cpp
//  Tonic
//
//  Created by Nick Donaldson on 3/10/13.

//

#include "ControlPulse.h"

namespace Tonic { namespace Tonic_{
  
  ControlPulse_::ControlPulse_() : lastOnTime_(0)
  {}
  
  void ControlPulse_::computeOutput(const SynthesisContext_ & context){
    
    ControlGeneratorOutput tickIn = input_.tick(context);
    ControlGeneratorOutput lengthIn = pulseLengthGen_.tick(context);
    
    lastOutput_.triggered = false;
    
    // every time input changes, reset status, start new pulse
    if (tickIn.triggered){
      state_ = ControlPulseStateOn;
      lastOnTime_ = context.elapsedTime;
      lastOutput_.triggered = true;
      lastOutput_.value = 1.0f;
    }
    else if (state_ == ControlPulseStateOn){
      
      double tDiff = context.elapsedTime - lastOnTime_;
      
      if (tDiff < 0 || tDiff >= max(0,lengthIn.value)){
        state_ = ControlPulseStateOff;
        lastOutput_.value = 0.0f;
        lastOutput_.triggered = true;
      }
    }
    
  }
  
  
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
