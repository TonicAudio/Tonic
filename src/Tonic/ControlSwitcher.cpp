//
//  ControlSwitcher.cpp
//  Tonic
//
//  Created by Morgan Packard on 4/11/13.
//
//

#include "ControlSwitcher.h"

namespace Tonic {
  
  namespace Tonic_{
  
    ControlSwitcher_::ControlSwitcher_() :
      inputIndex_(ControlValue(0)),
      lastInputIndex_(-1),
      doesWrap_(ControlValue(0)),
      addAfterWrap_(ControlValue(0)) {}
    
    void ControlSwitcher_::addInput(ControlGenerator input)
    {
      inputs_.push_back(input);
    }
    
    void ControlSwitcher_::setInputs(vector<ControlGenerator> inputs)
    {
      inputs_ = inputs;
    }
    
    void ControlSwitcher_::setDoesWrap(ControlGenerator doesWrap){
      doesWrap_ = doesWrap;
    }
    
    void ControlSwitcher_::setInputIndex(ControlGenerator inputIndexArg){
      inputIndex_ = inputIndexArg;
    }
    
    void ControlSwitcher_::setAddAfterWrap(ControlGenerator addAfterWrap){
      addAfterWrap_ = addAfterWrap;
    }
    
    void  setTriggerForIndex(ControlValue trigger, int index){
      
    }
    
  } // Namespace Tonic_
  
  
  ControlSwitcher & ControlSwitcher::setTriggerForIndex(ControlValue trigger, int index){
    return *this;
  }
  
  ControlSwitcher & ControlSwitcher::setFloatInputs(vector<float> inputs){
    
    vector<ControlGenerator> newInputs;
    
    for(vector<float>::iterator it = inputs.begin(); it != inputs.end(); it++){
      newInputs.push_back(ControlValue(*it));
    }
    
    gen()->setInputs(newInputs);
    
    return *this;
  }
  
} // Namespace Tonic
