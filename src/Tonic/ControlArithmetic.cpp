//
//  ControlArithmetic.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/24/13.

//

#include "ControlArithmetic.h"

namespace Tonic { namespace Tonic_{
    
  void ControlAdder_::input(ControlGenerator input){
    inputs.push_back(input);
  }

  void ControlMultiplier_::input(ControlGenerator input){
    inputs.push_back(input);
  }
  
} // Namespace Tonic_

} // Namespace Tonic
