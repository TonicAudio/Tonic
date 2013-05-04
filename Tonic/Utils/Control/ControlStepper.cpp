//
//  ControlStepper.cpp
//  Tonic
//
//  Created by Morgan Packard on 3/25/13.

//

#include "ControlStepper.h"

namespace Tonic { namespace Tonic_{
  
  ControlStepper_::ControlStepper_():hasBeenTriggered(false) {
      start = ControlValue(0);
      end = ControlValue(1);
      step = ControlValue(1);
      direction = 1;
      bidirectional = ControlValue(0);
   }
  
  ControlStepper_::~ControlStepper_(){
    
  }
  
  void ControlStepper_::computeOutput(const SynthesisContext_ & context){
    float startVal = start.tick(context).value;
    float endVal = end.tick(context).value;
    float stepVal = step.tick(context).value;
    bool bi = bidirectional.tick(context).value;
    
    lastOutput_.status = trigger.tick(context).status;
    if(hasBeenTriggered){
      if(lastOutput_.status == ControlGeneratorStatusHasChanged){
        lastOutput_.value += stepVal * direction;
        if (lastOutput_.value <= startVal) {
          lastOutput_.value = startVal;
          direction = 1;
        }else if(lastOutput_.value >= endVal){
            if(bi){
              direction = -1;
            }else{
              lastOutput_.value = startVal;
            }
        }
      }
    } else{
      // So first tick will output start value, even if it hasn't been triggered yet
      lastOutput_.value = startVal;
      hasBeenTriggered = true;
    }
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
