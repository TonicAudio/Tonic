//
//  ControlAdder.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/24/13.

//

#include "ControlAdder.h"

namespace Tonic { namespace Tonic_{
  
  void ControlAdder_::input(ControlGenerator input){
    inputs.push_back(input);
  }
  
  void ControlAdder_::computeOutput(const SynthesisContext_ &context){
    
    lastOutput_.status = ControlGeneratorStatusHasNotChanged;
    
    for (unsigned int i=0; i<inputs.size(); i++){
      if (inputs[i].tick(context).status == ControlGeneratorStatusHasChanged){
        lastOutput_.status = ControlGeneratorStatusHasChanged;
        break;
      }
    }
    
    TonicFloat sum = 0.0f;
    for (unsigned int i=0; i<inputs.size(); i++){
      sum += inputs[i].tick(context).value;
    }
    lastOutput_.value = sum;
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
