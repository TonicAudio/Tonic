//
//  ControlFloor.h
//  TonicDemo
//
//  Created by Morgan Packard on 3/4/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef __TonicDemo__ControlFloor__
#define __TonicDemo__ControlFloor__

#include <iostream>
#include "ControlConditioner.h"

namespace Tonic{

namespace Tonic_{

  class ControlFloor_ : public ControlConditioner_{
   
    void                    computeOutput(const SynthesisContext_ & context);
   
  };

}

  class ControlFloor : public TemplatedControlConditioner<ControlFloor, Tonic_::ControlFloor_>{
    
  };

}

#endif /* defined(__TonicDemo__ControlFloor__) */
