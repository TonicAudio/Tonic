//
//  ControlValue.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 2/12/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "ControlValue.h"
#include "RampedValue.h"

namespace Tonic {
  
  namespace Tonic_{
  
    ControlValue_::ControlValue_():
      mHasChanged(false),
      mLastHasChanged(false),
      mValue(0),
      mLastFrameIndex(0)
    {}
    
    ControlValue_::~ControlValue_(){}
    
    bool ControlValue_::hasChanged(const SynthesisContext_ & context){
      if (mLastFrameIndex == 0 || mLastFrameIndex != context.elapsedFrames){
        mLastFrameIndex = context.elapsedFrames;
        mLastHasChanged = mHasChanged;
        mHasChanged = false;
      }
      return mLastHasChanged;
    }
    
    TonicFloat ControlValue_::getValue(const SynthesisContext_ & context){
      return mValue;
    }
    
    TonicFloat ControlValue_::getValue(){
      return mValue;
    }
    
    void ControlValue_::setHasChanged(bool flagVal){
      mHasChanged = flagVal;
    }
    
    void ControlValue_::setValue(float value){
      mValue = value;
      mHasChanged = true;
    }
  
  }
  
  RampedValue ControlValue::ramped(float lenMs){
    return RampedValue(gen()->getValue(), lenMs).target(*this);
  }
  
}
