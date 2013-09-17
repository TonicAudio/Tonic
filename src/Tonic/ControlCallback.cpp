//
//  ControlCallback.cpp
//  Tonic
//
//  Created by Morgan Packard on 9/17/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//


#include "ControlCallback.h"
#ifdef TONIC_HAS_CPP_11

namespace Tonic { namespace Tonic_{
  
  ControlCallback_::ControlCallback_(){
    
  }
  
  ControlCallback_::~ControlCallback_(){
    
  }
  
  void ControlCallback_::computeOutput(const SynthesisContext_ & context){
    callback_(input_.tick(context));
  }
  
  void  ControlCallback_::setCallback(function<void(ControlGeneratorOutput)> fn){
    callback_ = fn;
  }
  
} // Namespace Tonic_
  
  ControlCallback::ControlCallback(Synth* synth, function<void(ControlGeneratorOutput)> fn ){
    synth->addAuxControlGenerator(*this);
    gen()->setCallback(fn);
  }
  
  
} // Namespace Tonic

#endif
