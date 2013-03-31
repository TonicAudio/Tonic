//
//  ControlStepper.cpp
//  Tonic
//
//  Created by Morgan Packard on 3/25/13.

//

#include "ControlStepper.h"

namespace Tonic { namespace Tonic_{
  
  ControlStepper_::ControlStepper_(){
      start = ControlValue(0);
      end = ControlValue(1);
      step = ControlValue(1);
      direction = 1;
   }
  
  ControlStepper_::~ControlStepper_(){
    
  }
  
  void ControlStepper_::computeOutput(const SynthesisContext_ & context){
    float startVal = start.tick(context).value;
    float endVal = end.tick(context).value;
    float stepVal = step.tick(context).value;
    lastOutput_.status = trigger.tick(context).status;
    if(lastOutput_.status == ControlGeneratorStatusHasChanged){
      lastOutput_.value += stepVal * direction;
      if (lastOutput_.value < startVal) {
        lastOutput_.value = startVal;
        direction = 1;
      }else if(lastOutput_.value > endVal){
        lastOutput_.value = endVal;
        direction = -1;
      }
    }
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
