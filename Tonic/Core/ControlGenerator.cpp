//
//  ControlGenerator.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 2/12/13.

//

#ifndef TonicDemo_ControlGenerator_cpp
#define TonicDemo_ControlGenerator_cpp

#include "ControlGenerator.h"
#include "RampedValue.h"
#include <cmath>

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
      
      #ifdef TONIC_DEBUG
      if(lastOutput_.value != lastOutput_.value){
        Tonic::error("ControlGenerator_::tick NaN detected.", true);
      }
      #endif
      
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
