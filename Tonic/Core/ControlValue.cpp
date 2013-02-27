//
//  ControlValue.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 2/12/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "ControlValue.h"

namespace Tonic {
  
  namespace Tonic_{
  
    ControlValue_::ControlValue_():
      hasChanged_(false),
      value_(0)
    {}
    
    ControlValue_::~ControlValue_(){}
    
    ControlGeneratorOutput ControlValue_::peek(){
      ControlGeneratorOutput peekOutput;
      peekOutput.value = value_;
      peekOutput.status = hasChanged_ ? ControlGeneratorStatusHasChanged : ControlGeneratorStatusHasNotChanged;
      return peekOutput;
    }
    
    void ControlValue_::setValue(float value){
      value_ = value;
      hasChanged_ = true;
    }
  
    ControlGeneratorStatus ControlValue_::computeStatus(const SynthesisContext_ & context){
      return hasChanged_ ? ControlGeneratorStatusHasChanged : ControlGeneratorStatusHasNotChanged;
    }
    
    TonicFloat ControlValue_::computeValue(const SynthesisContext_ &context){
      hasChanged_ = false;
      return value_;
    }
  }
  
}
