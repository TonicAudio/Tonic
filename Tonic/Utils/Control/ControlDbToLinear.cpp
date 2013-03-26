//
//  ControlDbToLinear.cpp
//  Tonic
//
//  Created by Nick Donaldson on 3/24/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "ControlDbToLinear.h"

namespace Tonic { namespace Tonic_{
  
  ControlDbToLinear_::ControlDbToLinear_(){
    
  }
  
  ControlDbToLinear_::~ControlDbToLinear_(){
    
  }
  
  void ControlDbToLinear_::computeOutput(const SynthesisContext_ & context){
    
    ControlGeneratorOutput inputOutput = input_.tick(context);
    lastOutput_.status = inputOutput.status;
    if (inputOutput.status == ControlGeneratorStatusHasChanged){
      lastOutput_.value = dBToLin(inputOutput.value);
    }
    
  }
  
} // Namespace Tonic_
  
} // Namespace Tonic