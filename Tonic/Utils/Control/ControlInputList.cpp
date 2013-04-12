//
//  ControlInputList.cpp
//  Tonic
//
//  Created by Morgan Packard on 4/11/13.
//
//

#include "ControlInputList.h"

namespace Tonic { namespace Tonic_{
  
  ControlInputList_::ControlInputList_() : inputIndex(ControlValue(0)){
    
  }
  
  ControlInputList_::~ControlInputList_(){
    
  }
  
  void ControlInputList_::addInput(ControlGenerator input){
    inputs.push_back(input);
  }
  
  void ControlInputList_::setInputIndex(ControlGenerator inputIndexArg){
    inputIndex = inputIndexArg;
  }
  
  
  void ControlInputList_::computeOutput(const SynthesisContext_ & context){
    int index = inputIndex.tick(context).value;
    for(vector<ControlGenerator>::iterator it = inputs.begin(); it != inputs.end(); it++){
      it->tick(context);
    }
    ControlGeneratorOutput output = inputs.at(index).tick(context);
    if (output.value != lastOutput_.value) {
      lastOutput_.status = ControlGeneratorStatusHasChanged;
      lastOutput_.value = output.value;
    }else{
      lastOutput_.status = ControlGeneratorStatusHasNotChanged;
    }
  }
  
  
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
