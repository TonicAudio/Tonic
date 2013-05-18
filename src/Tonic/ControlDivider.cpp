//
//  ControlDivider.cpp
//  Tonic
//
//  Created by Morgan Packard on 4/19/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "ControlDivider.h"

namespace Tonic { namespace Tonic_{

  ControlDivider_::ControlDivider_(){}
  ControlDivider_::~ControlDivider_(){}
  
  void ControlDivider_::computeOutput(const SynthesisContext_ & context){
    ControlGeneratorOutput leftOut = left.tick(context);
    ControlGeneratorOutput rightOut = right.tick(context);
    
    bool rightIsZero = rightOut.value == 0;
    if(rightIsZero){
      error("ControlGenerator divide by zero encountered. Returning last valid value");
    }
    bool noChange = leftOut.status == ControlGeneratorStatusHasNotChanged && rightOut.status == ControlGeneratorStatusHasNotChanged;
    if(rightIsZero || noChange){
      lastOutput_.status = ControlGeneratorStatusHasNotChanged;
    }else{
      lastOutput_.status = ControlGeneratorStatusHasChanged;
      lastOutput_.value = leftOut.value / rightOut.value;
    }
  }
  
  
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
