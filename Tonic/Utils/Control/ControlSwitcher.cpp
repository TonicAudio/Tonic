//
//  ControlSwitcher.cpp
//  Tonic
//
//  Created by Morgan Packard on 4/11/13.
//
//

#include "ControlSwitcher.h"

namespace Tonic { namespace Tonic_{
  
  ControlSwitcher_::ControlSwitcher_() : inputIndex(ControlValue(0)){
    
  }
  
  ControlSwitcher_::~ControlSwitcher_(){
    
  }
  
  void ControlSwitcher_::addInput(ControlGenerator input){
    inputs.push_back(input);
  }
  
  void ControlSwitcher_::setInputIndex(ControlGenerator inputIndexArg){
    inputIndex = inputIndexArg;
  }
  
  
  void ControlSwitcher_::computeOutput(const SynthesisContext_ & context){
    int index = inputIndex.tick(context).value;
    for(vector<ControlGenerator>::iterator it = inputs.begin(); it != inputs.end(); it++){
      it->tick(context);
    }
    ControlGeneratorOutput output = inputs.at(clamp(index, 0, inputs.size() -1 )).tick(context);
    if (output.value != lastOutput_.value) {
      lastOutput_.status = ControlGeneratorStatusHasChanged;
      lastOutput_.value = output.value;
    }else{
      lastOutput_.status = ControlGeneratorStatusHasNotChanged;
    }
  }
  
  
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
