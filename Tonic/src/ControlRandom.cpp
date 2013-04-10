//
//  ControlRandom.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 3/14/13.

//

#include "ControlRandom.h"

namespace Tonic{

  namespace Tonic_{
  
    void ControlRandom_::computeOutput(const SynthesisContext_ & context){
      if(trigger.tick(context).status == ControlGeneratorStatusHasChanged){
        lastOutput_.status = ControlGeneratorStatusHasChanged;
        lastOutput_.value = randomFloat(min.tick(context).value, max.tick(context).value);
      }else{
        lastOutput_.status = ControlGeneratorStatusHasNotChanged;
      }
    }
    
  }
}