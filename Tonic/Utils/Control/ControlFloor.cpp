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
      lastOutput_.value = (int)input_.tick(context).value;
      lastOutput_.status = input_.tick(context).status;
    }
    
  }
}
