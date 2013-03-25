//
//  ControlStepper.h
//  Tonic 
//
//  Created by Morgan Packard on 3/25/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

/*+++++++++++++++++++++ License ++++++++++++++++++++

Use this code for whatever you want. There are NO 
RESTRICTIONS WHATSOVER. Modify it, repackage it, 
sell it, get rich from it, whatever. Go crazy. If 
you want to make mehappy, contribute to this 
project, but feel free to just use the code as a 
starting point for whatever you like.

Note that Tonic is heavily indebted to STK
https://ccrma.stanford.edu/software/stk/

++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef __Tonic__ControlStepper__
#define __Tonic__ControlStepper__

#include "ControlGenerator.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlStepper_ : public ControlGenerator_{
      
    protected:
      
      ControlGenerator start;
      ControlGenerator end;
      ControlGenerator step;
      ControlGenerator trigger;
      int direction;
      
      void computeOutput(const SynthesisContext_ & context);
      
    public:
      ControlStepper_();
      ~ControlStepper_();
      void setStart(ControlGenerator arg){start = arg;}
      void setEnd(ControlGenerator arg){end = arg;}
      void setStep(ControlGenerator arg){step = arg;}
      void setTigger(ControlGenerator arg){trigger = arg;}
    };
    
  }
  
  class ControlStepper : public TemplatedControlGenerator<Tonic_::ControlStepper_>{
    
  public:
  
  createControlGeneratorSetters(ControlStepper, start, setStart);
  createControlGeneratorSetters(ControlStepper, end, setEnd);
  createControlGeneratorSetters(ControlStepper, step, setStep);
  createControlGeneratorSetters(ControlStepper, trigger, setTigger);

  };
}

#endif /* defined(__Tonic__ControlStepper__) */


