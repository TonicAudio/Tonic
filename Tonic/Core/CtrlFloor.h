//
//  CtrlFloor.h
//  TonicDemo
//
//  Created by Morgan Packard on 3/4/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef __TonicDemo__CtrlFloor__
#define __TonicDemo__CtrlFloor__

#include <iostream>
#include "ControlConditioner.h"

namespace Tonic{

namespace Tonic_{

  class CtrlFloor_ : public ControlConditioner_{
   
    ControlGeneratorStatus  computeStatus(const SynthesisContext_ & context);
    TonicFloat              computeValue(const SynthesisContext_ & context);
   
  };

}

  class CtrlFloor : public TemplatedControlConditioner<CtrlFloor, Tonic_::CtrlFloor_>{
    
  };

}

#endif /* defined(__TonicDemo__CtrlFloor__) */
