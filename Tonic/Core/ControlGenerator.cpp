//
//  ControlGenerator.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 2/12/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_ControlGenerator_cpp
#define TonicDemo_ControlGenerator_cpp

#include "ControlGenerator.h"
#include "RampedValue.h"

namespace Tonic{
  namespace Tonic_{
    
    ControlGenerator_::ControlGenerator_() :
      lastFrameIndex_(0)
    {
      pthread_mutex_init(&genMutex_, NULL);
    }

    ControlGenerator_::~ControlGenerator_(){
      pthread_mutex_destroy(&genMutex_);
    }

    ControlGeneratorOutput ControlGenerator_::tick(const SynthesisContext_ & context){
      if (lastFrameIndex_ == 0 || lastFrameIndex_ != context.elapsedFrames){
        lastFrameIndex_ = context.elapsedFrames;
        computeOutput(context);
      }
      return lastOutput_;
    }
    
    ControlGeneratorOutput ControlGenerator_::initialOutput(){
      // return output from passing in an initialized context
      return tick(SynthesisContext_());
    }
    
  }
  
  RampedValue ControlGenerator::ramped(float length){
    return RampedValue( mGen->initialOutput().value, length ).target(*this);
  }
  
}

#endif
