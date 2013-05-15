//
//  ControlValue.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 2/12/13.

//

#include "ControlValue.h"

namespace Tonic {
  
  namespace Tonic_{
  
    ControlValue_::ControlValue_():
      hasChanged_(false),
      value_(0)
    {}
        
    void ControlValue_::setValue(TonicFloat value){
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
