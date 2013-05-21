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


