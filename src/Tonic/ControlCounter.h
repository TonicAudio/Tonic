//
//  ControlCounter.h
//  Tonic 
//
//  Created by Morgan Packard on 4/15/13.
//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_CONTROLCOUNTER_H
#define TONIC_CONTROLCOUNTER_H

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
    
    inline void ControlCounter_::computeOutput(const SynthesisContext_ & context){
      ControlGeneratorOutput tickOut = trigger.tick(context);
      ControlGeneratorOutput endOut = end.tick(context);
      output_.triggered = tickOut.triggered;
      if (tickOut.triggered) {
        output_.value += 1;
        if(output_.value > endOut.value){
          output_.value = 0;
        }
      }
    }
    
  }
  
  class ControlCounter : public TemplatedControlGenerator<Tonic_::ControlCounter_>{
    
  public:
  
    TONIC_MAKE_CTRL_GEN_SETTERS(ControlCounter, trigger, setTrigger);
    TONIC_MAKE_CTRL_GEN_SETTERS(ControlCounter, end, setEnd);

  };
}

#endif


