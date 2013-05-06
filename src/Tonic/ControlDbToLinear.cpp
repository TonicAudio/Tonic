//
//  ControlDbToLinear.cpp
//  Tonic
//
//  Created by Nick Donaldson on 3/24/13.

//

#include "ControlDbToLinear.h"

namespace Tonic { namespace Tonic_{

  void ControlDbToLinear_::computeOutput(const SynthesisContext_ & context){
    
    ControlGeneratorOutput inputOutput = input_.tick(context);
    lastOutput_.status = inputOutput.status;
    if (inputOutput.status == ControlGeneratorStatusHasChanged){
      lastOutput_.value = dBToLin(inputOutput.value);
    }
    
  }
  
} // Namespace Tonic_
  
} // Namespace Tonic