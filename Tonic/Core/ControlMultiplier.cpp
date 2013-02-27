//
//  ControlMultiplier.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/25/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "ControlMultiplier.h"

namespace Tonic { namespace Tonic_{
  
  void ControlMultiplier_::in(ControlGenerator input){
    inputs.push_back(input);
  }
  
  ControlGeneratorStatus ControlMultiplier_::computeStatus(const SynthesisContext_ &context){
    for (unsigned int i=0; i<inputs.size(); i++){
      if (inputs[i].tick(context).status == ControlGeneratorStatusHasChanged){
        return ControlGeneratorStatusHasChanged;
      }
    }
    return ControlGeneratorStatusHasNotChanged;
  }
  
  TonicFloat ControlMultiplier_::computeValue(const SynthesisContext_ &context){
    TonicFloat product = inputs[0].tick(context).value;
    for (unsigned int i=1; i<inputs.size(); i++){
      product *= inputs[i].tick(context).value;
    }
    return product;
  }
  
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
