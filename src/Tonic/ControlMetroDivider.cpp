//
//  ControlMetroDivider.cpp
//  Tonic
//
//  Created by Nick Donaldson on 5/31/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "ControlMetroDivider.h"

namespace Tonic { namespace Tonic_{
  
  ControlMetroDivider_::ControlMetroDivider_() : tickCounter_(0) {
    divisonsGen_ = ControlValue(1);
    offsetGen_ = ControlValue(0);
    lastOutput_.value = 1.f;
  }
    
  void ControlMetroDivider_::computeOutput(const SynthesisContext_ & context){
  
    offset_ = (unsigned int)(offsetGen_.tick(context).value);
    unsigned int divisions = max(1, divisonsGen_.tick(context).value);
    
    lastOutput_.status = ControlGeneratorStatusHasNotChanged;
    
    if (input_.tick(context).status == ControlGeneratorStatusHasChanged)
    {
      unsigned int modcount = (tickCounter_++ + offset_) % divisions;
      if (modcount == 0){
        lastOutput_.status = ControlGeneratorStatusHasChanged;
      }
      
      if (tickCounter_ >= divisions) tickCounter_ = 0;
    }

  }
  
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
