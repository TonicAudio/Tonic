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
      triggered_(false),
      value_(0)
    {}
        
    void ControlValue_::setValue(TonicFloat value){
      value_ = value;
      triggered_ = true;
    }
  
    void ControlValue_::computeOutput(const SynthesisContext_ & context){
      output_.triggered =  (triggered_ || context.forceNewOutput);
      triggered_ = context.forceNewOutput; // if new output forced, don't reset changed status until next tick
      output_.value = value_;
    }
  }
  
}
