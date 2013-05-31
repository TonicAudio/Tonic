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
    lastOutput_.value = 1.f;
  }
    
  void ControlMetroDivider_::computeOutput(const SynthesisContext_ & context){
  
    unsigned int divisions = max(1, divisonsGen_.tick(context).value);
    if (input_.tick(context).status == ControlGeneratorStatusHasChanged)
    {
      tickCounter_ = (tickCounter_ % divisions);
      lastOutput_.status = (tickCounter_++ == 0) ? ControlGeneratorStatusHasChanged : ControlGeneratorStatusHasNotChanged;
    }
    else{
      lastOutput_.status = ControlGeneratorStatusHasNotChanged;
    }
  }
  
  
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
