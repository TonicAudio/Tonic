//
//  ControlCallback.cpp
//  Tonic
//
//  Created by Morgan Packard on 9/17/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//


#include "ControlCallback.h"
#if TONIC_HAS_CPP_11

namespace Tonic { 
  
  namespace Tonic_{
  
    ControlCallback_::ControlCallback_() : synthWasSet(false), hasBeenTicked(false){
    
    }
  
    ControlCallback_::~ControlCallback_(){
    
    }
  
    void ControlCallback_::computeOutput(const SynthesisContext_ & context){
      ControlGeneratorOutput inputOut = input_.tick(context);
    if (!hasBeenTicked)
    {
      lastOutputValue_ = inputOut.value;
      hasBeenTicked = true;
    }
  
      if(inputOut.triggered || (inputOut.value != lastOutputValue_) ){
        callback_(inputOut);
      lastOutputValue_ = inputOut.value;
      }
    }
  
    void  ControlCallback_::setCallback(function<void(ControlGeneratorOutput)> fn){
    if (!synthWasSet)
    {
      Tonic::error("You must call ControlCallback::setSynth before setting a callback function", true);
    }
  
      callback_ = fn;
    }
  
  } // Namespace Tonic_
  
  ControlCallback::ControlCallback(Synth synth, function<void(ControlGeneratorOutput)> fn ){
    synth.addAuxControlGenerator(*this);
    gen()->synthWasSet =  true;
  gen()->setCallback(fn);

  }

  ControlCallback::ControlCallback(Synth synth, function<void(ControlGeneratorOutput)> fn)
  {
    synth.addAuxControlGenerator(*this);
    gen()->setCallback(fn);
  }

  
} // Namespace Tonic

#endif
