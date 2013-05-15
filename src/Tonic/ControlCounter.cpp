//
//  ControlCounter.cpp
//  Tonic
//
//  Created by Morgan Packard on 4/15/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "ControlCounter.h"

namespace Tonic { namespace Tonic_{
  
  ControlCounter_::ControlCounter_(){
    end = ControlValue(std::numeric_limits<float>::infinity());
  }
  
  void ControlCounter_::computeOutput(const SynthesisContext_ & context){
    ControlGeneratorOutput tickOut = trigger.tick(context);
    ControlGeneratorOutput endOut = end.tick(context);
    lastOutput_.status = tickOut.status;
      if (tickOut.status == ControlGeneratorStatusHasChanged) {
        lastOutput_.value += 1;
        if(lastOutput_.value > endOut.value){
          lastOutput_.value = 0;
        }
      }
  }
  
  
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
