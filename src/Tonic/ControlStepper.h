//
//  ControlStepper.h
//  Tonic 
//
//  Created by Morgan Packard on 3/25/13.

//
// See LICENSE.txt for license and usage information.
//


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
      ControlGenerator bidirectional;
      int direction;
      bool hasBeenTriggered;
      
      void computeOutput(const SynthesisContext_ & context);
      
    public:
      ControlStepper_();
      ~ControlStepper_();
      void setStart(ControlGenerator arg){start = arg;}
      void setEnd(ControlGenerator arg){end = arg;}
      void setStep(ControlGenerator arg){step = arg;}
      void setTigger(ControlGenerator arg){trigger = arg;}
      void setBidirectional(ControlGenerator arg){bidirectional = arg;}
    };
    
  }
  
  //! On trigger, outputs a value beginning at start, and incrementing by step toward end, then back again.
  // Currently it steps up and down. In the future an option may be added to snap back to the beginning.
  
  class ControlStepper : public TemplatedControlGenerator<Tonic_::ControlStepper_>{
    
  public:
  
  createControlGeneratorSetters(ControlStepper, start, setStart);
  createControlGeneratorSetters(ControlStepper, end, setEnd);
  createControlGeneratorSetters(ControlStepper, step, setStep);
  createControlGeneratorSetters(ControlStepper, trigger, setTigger);
  createControlGeneratorSetters(ControlStepper, bidirectional, setBidirectional);

  };
}

#endif /* defined(__Tonic__ControlStepper__) */


