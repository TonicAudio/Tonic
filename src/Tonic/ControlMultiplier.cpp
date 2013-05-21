//
//  ControlMultiplier.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/25/13.

//

#include "ControlMultiplier.h"

namespace Tonic { namespace Tonic_{
  
  void ControlMultiplier_::input(ControlGenerator input){
    inputs.push_back(input);
  }
  
  void ControlMultiplier_::computeOutput(const SynthesisContext_ &context){
    
    lastOutput_.status = ControlGeneratorStatusHasNotChanged;
    
    for (unsigned int i=0; i<inputs.size(); i++){
      if (inputs[i].tick(context).status == ControlGeneratorStatusHasChanged){
        lastOutput_.status = ControlGeneratorStatusHasChanged;
        break;
      }
    }
      
    TonicFloat product = inputs[0].tick(context).value;
    for (unsigned int i=1; i<inputs.size(); i++){
      product *= inputs[i].tick(context).value;
    }
    lastOutput_.value = product;
  }
  
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
