//
//  ControlSwitcher.cpp
//  Tonic
//
//  Created by Morgan Packard on 4/11/13.
//
//

#include "ControlSwitcher.h"

namespace Tonic { namespace Tonic_{
  
  ControlSwitcher_::ControlSwitcher_() : inputIndex_(ControlValue(0)), lastInputIndex_(-1) {
    
  }
  
  void ControlSwitcher_::addInput(ControlGenerator input){
    inputs_.push_back(input);
  }
  
  void ControlSwitcher_::setInputs(vector<ControlGenerator> inputs)
  {
    inputs_ = inputs;
  }
  
  void ControlSwitcher_::setInputIndex(ControlGenerator inputIndexArg){
    inputIndex_ = inputIndexArg;
  }
  
  void ControlSwitcher_::computeOutput(const SynthesisContext_ & context){
    
    for(vector<ControlGenerator>::iterator it = inputs_.begin(); it != inputs_.end(); it++){
      it->tick(context);
    }
    
    ControlGeneratorOutput indexOutput = inputIndex_.tick(context);
    int index = indexOutput.value;

    // always send has changed message when input index changes
    if (indexOutput.status == ControlGeneratorStatusHasChanged && index != lastInputIndex_) {
      lastInputIndex_ = index;
      ControlGeneratorOutput output = inputs_.at(clamp(index, 0, inputs_.size() -1 )).tick(context);
      lastOutput_.status = ControlGeneratorStatusHasChanged;
      lastOutput_.value = output.value;
    }else{
      lastOutput_.status = ControlGeneratorStatusHasNotChanged;
    }
  }
  
  
} // Namespace Tonic_
  
  ControlSwitcher & ControlSwitcher::setFloatInputs(vector<float> inputs){
    
    vector<ControlGenerator> newInputs;
    
    for(vector<float>::iterator it = inputs.begin(); it != inputs.end(); it++){
      newInputs.push_back(ControlValue(*it));
    }
    
    gen()->setInputs(newInputs);
    
    return *this;
  }
  
} // Namespace Tonic
