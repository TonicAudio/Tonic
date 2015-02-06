//
//  ControlTrigger.cpp
//  Tonic
//
//  Created by Morgan Packard on 3/31/13.
//

#include "ControlTrigger.h"

namespace Tonic { namespace Tonic_{
  
  ControlTrigger_::ControlTrigger_() : doTrigger(false){
    
  }
  
  void ControlTrigger_::trigger(float value){
    doTrigger = true;
    output_.value = value;
  }
  
} // Namespace Tonic_
  
  
  ControlTrigger&  ControlTrigger::trigger(float value){
    gen()->trigger(value);
	return *this;
  }
  
} // Namespace Tonic
