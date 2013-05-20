//
//  ControlMetro.cpp
//  Tonic
//
//  Created by Nick Donaldson on 3/10/13.

//

#include "ControlMetro.h"

namespace Tonic { namespace Tonic_{
  
  ControlMetro_::ControlMetro_() : lastClickTime_(0) {

  }
  
  void ControlMetro_::computeOutput(const SynthesisContext_ & context){
    
    double sPerBeat = 60.0/max(0.001,bpm_.tick(context).value);
    double delta = context.elapsedTime - lastClickTime_;
    if (delta >= 2*sPerBeat || delta < 0){
      // account for bpm interval outrunning tick interval or timer wrap-around
      lastClickTime_ = context.elapsedTime;
      lastOutput_.status = ControlGeneratorStatusHasChanged;
    }
    else if (delta >= sPerBeat){
      // acocunt for drift
      lastClickTime_ += sPerBeat;
      lastOutput_.status = ControlGeneratorStatusHasChanged;
    }
    else{
      lastOutput_.status = ControlGeneratorStatusHasNotChanged;
    }
    
    lastOutput_.value = 1;
    
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
