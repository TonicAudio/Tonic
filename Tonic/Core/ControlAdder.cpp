//
//  ControlAdder.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/24/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "ControlAdder.h"

namespace Tonic { namespace Tonic_{
  
  void ControlAdder_::in(ControlGenerator input){
    inputs.push_back(input);
  }
  
  ControlGeneratorStatus ControlAdder_::computeStatus(const SynthesisContext_ &context){
    for (unsigned int i=0; i<inputs.size(); i++){
      if (inputs[i].tick(context).status == ControlGeneratorStatusHasChanged){
        return ControlGeneratorStatusHasChanged;
      }
    }
    return ControlGeneratorStatusHasNotChanged;
  }
  
  TonicFloat ControlAdder_::computeValue(const SynthesisContext_ &context){
    TonicFloat sum = 0.0f;
    for (unsigned int i=0; i<inputs.size(); i++){
      sum += inputs[i].tick(context).value;
    }
    return sum;
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
