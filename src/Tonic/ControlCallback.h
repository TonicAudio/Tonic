//
//  ControlCallback.h
//  Tonic 
//
//  Created by Morgan Packard on 9/17/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ControlCallback__
#define __Tonic__ControlCallback__



#include "ControlConditioner.h"

#ifdef TONIC_HAS_CPP_11
#include <functional>
#include "Synth.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlCallback_ : public ControlConditioner_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
      function<void(ControlGeneratorOutput)> callback_;
      
    public:
      ControlCallback_();
      ~ControlCallback_();
      void setCallback(function<void(ControlGeneratorOutput)> fn);
      
    };
    
  }
  
  class ControlCallback : public TemplatedControlConditioner<ControlCallback, Tonic_::ControlCallback_>{
    
  public:
  
    ControlCallback(Synth* synth, function<void(ControlGeneratorOutput)>);
    

  };
}

#endif

#endif /* defined(__Tonic__ControlCallback__) */


