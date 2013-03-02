//
//  ADSR.cpp
//  Tonic
//
//  Created by Morgan Packard on 2/28/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "ADSR.h"

namespace Tonic { namespace Tonic_{
  
  ADSR_::ADSR_() : state(ATTACK){
    ramp.lengthMs(1000);
    ramp.target(0);
    ramp.value(0);
  }
  
  ADSR_::~ADSR_(){
    
  }
  
  void ADSR_::switchState(State newState, const SynthesisContext_ &context){
    state = newState;
    switch(state){
      case ATTACK:
        ramp.value(0);
        ramp.target(mTrigger.tick(context).value);
        ramp.lengthMs(attack.tick(context).value);
      break;
      case DECAY:
        ramp.target(sustain.tick(context).value);
        ramp.lengthMs(decay.tick(context).value);
        ramp.value(mTrigger.tick(context).value);
      break;
      case SUSTAIN:
        ramp.target(sustain.tick(context).value);
        ramp.value(sustain.tick(context).value);
      break;
      case RELEASE:
        ramp.target(0);
        ramp.lengthMs(release.tick(context).value);
      break;
      
      default:
      break;
    }
  }
  
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
