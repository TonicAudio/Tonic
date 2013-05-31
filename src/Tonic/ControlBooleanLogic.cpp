//
//  ControlBooleanLogic.cpp
//  Tonic
//
//  Created by Nick Donaldson on 5/31/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "ControlBooleanLogic.h"

namespace Tonic { namespace Tonic_{
  
  ControlGreaterThan_::ControlGreaterThan_() : rhsGen_(ControlValue(0)) {};
  
  void ControlGreaterThan_::computeOutput(const SynthesisContext_ & context){
  
    lastOutput_.status = ControlGeneratorStatusHasNotChanged;
    
    ControlGeneratorOutput lhsOut = input_.tick(context);
    ControlGeneratorOutput rhsOut = rhsGen_.tick(context);
    if (lhsOut.status == ControlGeneratorStatusHasChanged || rhsOut.status == ControlGeneratorStatusHasChanged){
      lastOutput_.value = lhsOut.value > rhsOut.value ? 1.0f : 0.0f;
      lastOutput_.status = ControlGeneratorStatusHasChanged;
    }
    else{
      lastOutput_.value = 0;
    }
    
  }

  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
