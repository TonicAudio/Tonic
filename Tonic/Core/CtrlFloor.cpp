//
//  CtrlFloor.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 3/4/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "CtrlFloor.h"

namespace Tonic{

  namespace Tonic_{
  
    ControlGeneratorStatus  CtrlFloor_::computeStatus(const SynthesisContext_ & context){
      return input_.tick(context).status;
    }
    
    TonicFloat CtrlFloor_::computeValue(const SynthesisContext_ & context){
      return (int)input_.tick(context).value;
    }
    
  }
}
