//
//  ControlTriggerFilter.h
//  Tonic 
//
//  Created by Morgan Packard on 5/22/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ControlTriggerFilter__
#define __Tonic__ControlTriggerFilter__

#include "ControlGenerator.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlTriggerFilter_ : public ControlGenerator_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
      vector<bool> sequence_;
      ControlGenerator trigger_;
      int step;
      
    public:
      ControlTriggerFilter_();
      ~ControlTriggerFilter_();
      void sequence(string);
      void trigger(ControlGenerator);
      
    };
    
  }
  
  /*!
    Passes through, or blocks trigger messages based on a string-encoded sequence of "0" and "1" passed to the "sequence" method.
    "1110" will result in four passes and one block. "111000" will result in three passes and three blocks. Any character other than "1" and "0" will be ignored. So you can add delimiters for readability, like this: "1110|1000|1010|1111". "1" means pass, "0" means block.
  */
  
  class ControlTriggerFilter : public TemplatedControlGenerator<Tonic_::ControlTriggerFilter_>{
    
  public:
  
  TONIC_MAKE_CTRL_GEN_SETTERS(ControlTriggerFilter, trigger, trigger)
  ControlTriggerFilter sequence(string seqArg){gen()->sequence(seqArg); return *this;};

  };
}

#endif /* defined(__Tonic__ControlTriggerFilter__) */


