//
//  ControlPulse.h
//  Tonic 
//
//  Created by Nick Donaldson on 3/10/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ControlPulse__
#define __Tonic__ControlPulse__

#include "ControlConditioner.h"

namespace Tonic {
  
  namespace Tonic_ {
    

    class ControlPulse_ : public ControlConditioner_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
    
      typedef enum {
        ControlPulseStateOff = 0,
        ControlPulseStateOn
      } ControlPulseState;
      
      ControlPulseState state_;
      double lastOnTime_;
      
      ControlGenerator pulseLengthGen_;
      
    public:
      
      ControlPulse_();      
      void setPulseLengthGen( ControlGenerator gen ){ pulseLengthGen_ = gen; };
    
    };
    
    inline   void ControlPulse_::computeOutput(const SynthesisContext_ & context){
      
      ControlGeneratorOutput tickIn = input_.tick(context);
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
  
  class ControlPulse : public TemplatedControlConditioner<ControlPulse, Tonic_::ControlPulse_>{
    
  public:
    
    ControlPulse(float length = 0.1){
      gen()->setPulseLengthGen(ControlValue(length));
    }
    
    createControlGeneratorSetters(ControlPulse, length, setPulseLengthGen);
    
  };
}

#endif /* defined(__Tonic__ControlPulse__) */


