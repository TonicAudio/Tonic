//
//  ControlFloor.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 3/4/13.

//

#include "ControlFloor.h"

namespace Tonic{

  namespace Tonic_{
    void ControlFloor_::computeOutput(const SynthesisContext_ & context){
      output_.value = (int)input_.tick(context).value;
      output_.triggered = input_.tick(context).triggered;
    }
    
  }
}
