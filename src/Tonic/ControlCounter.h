//
//  ControlCounter.h
//  Tonic 
//
//  Created by Morgan Packard on 4/15/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ControlCounter__
#define __Tonic__ControlCounter__

#include "ControlGenerator.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlCounter_ : public ControlGenerator_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
      ControlGenerator trigger;
      ControlGenerator end;
      
    public:

      ControlCounter_();
      
      void setTrigger(ControlGenerator gen){ trigger = gen;};
      void setEnd(ControlGenerator gen){end = gen;};
      
    };
    
  }
  
  class ControlCounter : public TemplatedControlGenerator<Tonic_::ControlCounter_>{
    
  public:
  
    createControlGeneratorSetters(ControlCounter, trigger, setTrigger);
    createControlGeneratorSetters(ControlCounter, end, setEnd);

  };
}

#endif /* defined(__Tonic__ControlCounter__) */


