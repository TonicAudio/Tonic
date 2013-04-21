//
//  ControlTrigger.cpp
//  Tonic
//
//  Created by Morgan Packard on 3/31/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "ControlTrigger.h"

namespace Tonic { namespace Tonic_{
  
  ControlTrigger_::ControlTrigger_() : doTrigger(false){
    
  }
  
  ControlTrigger_::~ControlTrigger_(){
    
  }
  
  void ControlTrigger_::computeOutput(const SynthesisContext_ & context){
    lastOutput_.status = doTrigger ? ControlGeneratorStatusHasChanged : ControlGeneratorStatusHasNotChanged;
    doTrigger = false;
  }
  
  void  ControlTrigger_::trigger(float value){
      doTrigger = true;
      lastOutput_.value = value;
  }
  
  
  
} // Namespace Tonic_
  
  
  void  ControlTrigger::trigger(float value){
    gen()->trigger(value);
  }
  
} // Namespace Tonic
