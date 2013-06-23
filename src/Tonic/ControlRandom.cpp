//
//  ControlRandom.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 3/14/13.

//

#include "ControlRandom.h"

namespace Tonic{

  namespace Tonic_{
  
    ControlRandom_::ControlRandom_(){
      trigger = ControlValue(0);
      min = ControlValue(0);
      max = ControlValue(1);
    }
    
  }
}