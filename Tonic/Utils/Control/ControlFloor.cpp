//
//  ControlFloor.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 3/4/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
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
