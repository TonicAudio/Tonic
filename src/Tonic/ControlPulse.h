//
//  ControlPulse.h
//  Tonic 
//
//  Created by Nick Donaldson on 3/10/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_CONTROLPULSE_H
#define TONIC_CONTROLPULSE_H

#include "ControlGenerator.h"

namespace Tonic {
  
  namespace Tonic_ {
    

    class ControlPulse_ : public ControlGenerator_ {
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
    
      typedef enum {
        ControlPulseStateOff = 0,
        ControlPulseStateOn
      } ControlPulseState;
      
      ControlPulseState state_;
      double lastOnTime_;
      
      ControlGenerator triggerGen_;
      ControlGenerator pulseLengthGen_;
      
    public:
      
      ControlPulse_();
      
      void setTriggerGen( ControlGenerator gen ){ triggerGen_ = gen; };
      void setPulseLengthGen( ControlGenerator gen ){ pulseLengthGen_ = gen; };
    
    };
    
    inline   void ControlPulse_::computeOutput(const SynthesisContext_ & context){
      
      ControlGeneratorOutput tickIn = triggerGen_.tick(context);
      ControlGeneratorOutput lengthIn = pulseLengthGen_.tick(context);
      
      output_.triggered = false;
      
      // every time input changes, reset status, start new pulse
      if (tickIn.triggered){
        state_ = ControlPulseStateOn;
        lastOnTime_ = context.elapsedTime;
        output_.triggered = true;
        output_.value = 1.0f;
      }
      else if (state_ == ControlPulseStateOn){
        
        double tDiff = context.elapsedTime - lastOnTime_;
        
        if (tDiff < 0 || tDiff >= max(0,lengthIn.value)){
          state_ = ControlPulseStateOff;
          output_.value = 0.0f;
          output_.triggered = true;
        }
      }
      
    }
    
  }
  
  class ControlPulse : public TemplatedControlGenerator<Tonic_::ControlPulse_> {
    
    public:
      
      ControlPulse(float length = 0.1){
        gen()->setPulseLengthGen(ControlValue(length));
      }
    
      TONIC_MAKE_CTRL_GEN_SETTERS(ControlPulse, trigger, setTriggerGen);
      TONIC_MAKE_CTRL_GEN_SETTERS(ControlPulse, length, setPulseLengthGen);
      
  };
}

#endif


