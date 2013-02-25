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
      hasChanged_(false),
      value_(0)
    {}
    
    ControlValue_::~ControlValue_(){}
    
    
    TonicFloat ControlValue_::getValue(){
      return value_;
    }
    
    void ControlValue_::setHasChanged(bool flagVal){
      hasChanged_ = flagVal;
    }
    
    void ControlValue_::setValue(float value){
      value_ = value;
      hasChanged_ = true;
    }
  
    bool ControlValue_::computeHasChanged(const SynthesisContext_ &context){
      return hasChanged_;
    }
    
    TonicFloat ControlValue_::computeValue(const SynthesisContext_ &context){
      hasChanged_ = false;
      return value_;
    }
  }
  
  RampedValue ControlValue::ramped(float lenMs){
    return RampedValue(gen()->getValue(), lenMs).target(*this);
  }
  
}
