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
    
    void ControlValue_::setValue(float value){
      value_ = value;
      hasChanged_ = true;
    }
  
    void ControlValue_::computeOutput(const SynthesisContext_ & context){
      lastOutput_.status =  hasChanged_ ? ControlGeneratorStatusHasChanged : ControlGeneratorStatusHasNotChanged;
      hasChanged_ = false;
      lastOutput_.value = value_;
    }
  }
  
}
