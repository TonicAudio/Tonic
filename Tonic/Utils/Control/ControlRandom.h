//
//  ControlRandom.h
//  TonicDemo
//
//  Created by Morgan Packard on 3/14/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef __TonicDemo__ControlRandom__
#define __TonicDemo__ControlRandom__

#include <iostream>

#include <iostream>
#include "ControlConditioner.h"

namespace Tonic{

namespace Tonic_{

  class ControlRandom_ : public ControlGenerator_{
   
    void computeOutput(const SynthesisContext_ & context);
    public:
    
    ControlGenerator max;
    ControlGenerator min;
    ControlGenerator trigger;
    
    void setMax(ControlGenerator maxArg){max = maxArg;};
    void setMin(ControlGenerator minArg){min = minArg;};
    void setTrigger(ControlGenerator arg){trigger = arg;}
  };

}

  class ControlRandom : public TemplatedControlGenerator<Tonic_::ControlRandom_>{
  public:
    createControlGeneratorSetters(ControlRandom, max, setMax)
    createControlGeneratorSetters(ControlRandom, min, setMin)
    createControlGeneratorSetters(ControlRandom, trigger, setTrigger)
  };

}


#endif /* defined(__TonicDemo__ControlRandom__) */
