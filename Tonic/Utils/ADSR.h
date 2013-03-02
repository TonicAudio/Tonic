//
//  ADSR.h
//  Tonic 
//
//  Created by Morgan Packard on 2/28/13.
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

#ifndef __Tonic__ADSR__
#define __Tonic__ADSR__

#include <iostream>
#include "Generator.h"
#include "ControlGenerator.h"
#include "RampedValue.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ADSR_ : public Generator_{
      
    protected:
    
      void trigger();
    
      RampedValue decay;
      ControlGenerator mTrigger;
      
    public:
      ADSR_();
      ~ADSR_();
      void computeSynthesisBlock( const SynthesisContext_ &context );
      void setTrigger(ControlGenerator trig){mTrigger = trig;}
      
    };
    
    inline void ADSR_::computeSynthesisBlock(const SynthesisContext_ &context){
      ControlGeneratorOutput output = mTrigger.tick(context);
      if(output.status == ControlGeneratorStatusHasChanged && output.value != 0){
        decay.value(output.value);
        decay.target(0);
      }
      decay.tick(synthesisBlock_, context);
    }
    
  }
  
  class ADSR : public TemplatedGenerator<Tonic_::ADSR_>{
    
  public:
    createControlGeneratorSetters(ADSR, setTrigger, setTrigger);

  };
}

#endif /* defined(__Tonic__ADSR__) */


