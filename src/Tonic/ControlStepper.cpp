//
//  ControlStepper.cpp
//  Tonic
//
//  Created by Morgan Packard on 3/25/13.

//

#include "ControlStepper.h"

namespace Tonic { namespace Tonic_{
  
  ControlStepper_::ControlStepper_():hasBeenTriggered(false) {
      start = ControlValue(0);
      end = ControlValue(1);
      step = ControlValue(1);
      direction = 1;
      bidirectional = ControlValue(0);
   }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
