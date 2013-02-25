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
  
  bool ControlMultiplier_::computeHasChanged(const SynthesisContext_ &context){
    for (unsigned int i=0; i<inputs.size(); i++){
      if (inputs[i].hasChanged(context)){
        return true;
      }
    }
    return false;
  }
  
  TonicFloat ControlMultiplier_::computeValue(const SynthesisContext_ &context){
    TonicFloat product = inputs[0].getValue(context);
    for (unsigned int i=1; i<inputs.size(); i++){
      product *= inputs[i].getValue(context);
    }
    return product;
  }
  
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
