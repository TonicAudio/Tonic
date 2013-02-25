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
      lastHasChanged_(false),
      lastValue_(0),
      lastFrameIndex_HasChanged_(0),
      lastFrameIndex_Value_(0)
    {
      pthread_mutex_init(&genMutex_, NULL);
    }

    ControlGenerator_::~ControlGenerator_(){
      pthread_mutex_destroy(&genMutex_);
    }

    bool ControlGenerator_::hasChanged(const SynthesisContext_ & context){
      if (lastFrameIndex_HasChanged_ == 0 || lastFrameIndex_HasChanged_ != context.elapsedFrames){
        lastFrameIndex_HasChanged_ = context.elapsedFrames;
        lastHasChanged_ = computeHasChanged(context);
      }
      return lastHasChanged_;
    }
    
    TonicFloat ControlGenerator_::getValue(const SynthesisContext_ & context){
      if (lastFrameIndex_Value_ == 0 || lastFrameIndex_Value_ != context.elapsedFrames){
        lastFrameIndex_Value_ = context.elapsedFrames;
        lastValue_ = computeValue(context);
      }
      return lastValue_;
    }
    
  }
  
  RampedValue ControlGenerator::ramped(float lenMs){
    return RampedValue( mGen->getLastValue(), lenMs ).target(*this);
  }
  
}

#endif
