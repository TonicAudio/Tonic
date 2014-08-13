//
//  ControlCallback.h
//  Tonic 
//
//  Created by Morgan Packard on 9/17/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_CONTROLCALLBACK_H
#define TONIC_CONTROLCALLBACK_H

#include "ControlConditioner.h"

#if TONIC_HAS_CPP_11
#include <functional>
#include "Synth.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlCallback_ : public ControlConditioner_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
      function<void(ControlGeneratorOutput)> callback_;
      
    public:
      void setCallback(function<void(ControlGeneratorOutput)> fn);
      
    };
    
  }
  
  /*!
    Pass a lambda function to a controlgenerator. That function will be called when the input ControlGenerator sends a "triggered" message. Constructor takes two arguments -- the synth that owns the ControlCallback, and a lambda function
  */
  
  class ControlCallback : public TemplatedControlConditioner<ControlCallback, Tonic_::ControlCallback_>{
    
  public:
  
    ControlCallback(Synth* synth, function<void(ControlGeneratorOutput)>);
    

  };
}

#endif

#endif


