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
    ramp.length(1.0);
    ramp.target(0);
    ramp.value(0);
    isLegato = ControlValue(0);
    sustains = ControlValue(true);
    
  }
  
  ADSR_::~ADSR_(){
    
  }
  
  void ADSR_::switchState(State newState, const SynthesisContext_ &context){
    state = newState;
    switch(state){
      case ATTACK:{
        float rampStart = isLegato.tick(context).value == 0 ? 0 : synthesisBlock_[synthesisBlock_.frames() - 1];
        ramp.value(rampStart);
        ramp.target(1);
        ramp.length(attack.tick(context).value);
      }
      break;
      case DECAY:
        ramp.target(sustain.tick(context).value);
        ramp.length(decay.tick(context).value);
      break;
      case SUSTAIN:
        ramp.value(sustain.tick(context).value);
      break;
      case RELEASE:
        ramp.target(0);
        ramp.length(release.tick(context).value);
      break;
      
      default:
      break;
    }
  }
  
  
} // Namespace Tonic_


  ADSR::ADSR(){}  
  
  ADSR::ADSR(float attackArg, float decayArg, float sustainArg, float releaseArg){
    attack(attackArg);
    decay(decayArg);
    sustain(sustainArg);
    release(releaseArg);
  }

  
} // Namespace Tonic
