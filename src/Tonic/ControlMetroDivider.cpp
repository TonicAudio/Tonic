//
//  ControlMetroDivider.cpp
//  Tonic
//
//  Created by Nick Donaldson on 5/31/13.
//

#include "ControlMetroDivider.h"

namespace Tonic { namespace Tonic_{
  
  ControlMetroDivider_::ControlMetroDivider_() : tickCounter_(0) {
    divisonsGen_ = ControlValue(1);
    offsetGen_ = ControlValue(0);
    output_.value = 1.f;
  }
  
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
