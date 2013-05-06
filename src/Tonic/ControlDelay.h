//
//  ControlDelay.h
//  Tonic 
//
//  Created by Nick Donaldson on 5/5/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
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

#ifndef __Tonic__ControlDelay__
#define __Tonic__ControlDelay__

#include "ControlConditioner.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlDelay_ : public ControlConditioner_{
      
    private:

      long readHead_;
      long writeHead_;
          
      long maxDelay_; // # synthesis blocks of delay
      
      std::vector<ControlGeneratorOutput> delayLine_;
      
      ControlGenerator delayTimeCtrlGen_;
      
      void computeOutput(const SynthesisContext_ & context);
      
    public:
      
      ControlDelay_();
      
      void initialize(float maxDelayTime);
      
      void setDelayTimeGen( ControlGenerator gen ){ delayTimeCtrlGen_ = gen; };
      
    };
    
  }
  
  class ControlDelay : public TemplatedControlConditioner<ControlDelay, Tonic_::ControlDelay_>{
    
  public:
    
    ControlDelay(float maxDelayTime = 1.0f);
    createControlGeneratorSetters(ControlDelay, delayTime, setDelayTimeGen);
    
  };
}

#endif /* defined(__Tonic__ControlDelay__) */


