//
//  ControlRandom.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 3/14/13.

//

#include "ControlRandom.h"

namespace Tonic{

  namespace Tonic_{
  
    ControlRandom_::ControlRandom_(){
      trigger = ControlValue(0);
    }
  
    void ControlRandom_::computeOutput(const SynthesisContext_ & context){
      ControlGeneratorOutput minOut = min.tick(context);
      ControlGeneratorOutput maxOut = max.tick(context);
    
      bool outInRange =  (lastOutput_.value >= minOut.value) && (lastOutput_.value <= maxOut.value);
    
      if(!outInRange || trigger.tick(context).status == ControlGeneratorStatusHasChanged){
        lastOutput_.status = ControlGeneratorStatusHasChanged;
        lastOutput_.value = randomFloat(minOut.value, maxOut.value);
      }else{
        lastOutput_.status = ControlGeneratorStatusHasNotChanged;
      }
    }
    
  }
}