//
//  ControlStepper.h
//  Tonic 
//
//  Created by Morgan Packard on 3/25/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_CONTROLSTEPPER_H
#define TONIC_CONTROLSTEPPER_H

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
      void setStart(ControlGenerator arg){start = arg;}
      void setEnd(ControlGenerator arg){end = arg;}
      void setStep(ControlGenerator arg){step = arg;}
      void setTigger(ControlGenerator arg){trigger = arg;}
      void setBidirectional(ControlGenerator arg){bidirectional = arg;}
    };
    
    inline void ControlStepper_::computeOutput(const SynthesisContext_ & context){
      float startVal = start.tick(context).value;
      float endVal = end.tick(context).value;
      float stepVal = step.tick(context).value;
      bool bi = bidirectional.tick(context).value;
      
      output_.triggered = trigger.tick(context).triggered;
      if(hasBeenTriggered){
        if(output_.triggered){
          output_.value += stepVal * direction;
          if (output_.value <= startVal) {
            output_.value = startVal;
            direction = 1;
          }else if(output_.value >= endVal){
            if(bi){
              direction = -1;
            }else{
              output_.value = startVal;
            }
          }
        }
      } else{
        // So first tick will output start value, even if it hasn't been triggered yet
        output_.value = startVal;
        hasBeenTriggered = true;
      }
    }
    
  }
  
  //! On trigger, outputs a value beginning at start, and incrementing by step toward end, then back again.
  // Currently it steps up and down. In the future an option may be added to snap back to the beginning.
  
  class ControlStepper : public TemplatedControlGenerator<Tonic_::ControlStepper_>{
    
  public:
  
  TONIC_MAKE_CTRL_GEN_SETTERS(ControlStepper, start, setStart);
  TONIC_MAKE_CTRL_GEN_SETTERS(ControlStepper, end, setEnd);
  TONIC_MAKE_CTRL_GEN_SETTERS(ControlStepper, step, setStep);
  TONIC_MAKE_CTRL_GEN_SETTERS(ControlStepper, trigger, setTigger);
  TONIC_MAKE_CTRL_GEN_SETTERS(ControlStepper, bidirectional, setBidirectional);

  };
}

#endif


