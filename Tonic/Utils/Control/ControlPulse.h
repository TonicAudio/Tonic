//
//  ControlPulse.h
//  Tonic 
//
//  Created by Nick Donaldson on 3/10/13.
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
      ~ControlPulse_();
      
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


