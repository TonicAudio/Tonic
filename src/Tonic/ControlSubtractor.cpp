//
//  ControlSubtractor.cpp
//  Tonic
//
//  Created by Morgan Packard on 4/19/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "ControlSubtractor.h"

namespace Tonic { namespace Tonic_{
  
  ControlSubtractor_::ControlSubtractor_(){
    
  }
  
  ControlSubtractor_::~ControlSubtractor_(){
    
  }
  
  void ControlSubtractor_::computeOutput(const SynthesisContext_ & context){
    ControlGeneratorOutput leftOut = left.tick(context);
    ControlGeneratorOutput rightOut = right.tick(context);
    if(leftOut.status == ControlGeneratorStatusHasNotChanged && rightOut.status == ControlGeneratorStatusHasNotChanged){
      lastOutput_.status = ControlGeneratorStatusHasNotChanged;
    }else{
      lastOutput_.status = ControlGeneratorStatusHasChanged;
      lastOutput_.value = leftOut.value - rightOut.value;
    }
  }
  
  
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
