//
//  ADSR.cpp
//  Tonic
//
//  Created by Morgan Packard on 2/28/13.

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
  
  void ADSR_::switchState(State newState){
    state = newState;
    switch(state){
      case ATTACK:{
        float rampStart = legatoVal ? synthesisBlock_[synthesisBlock_.frames() - 1] : 0;
        ramp.value(rampStart);
        ramp.target(1);
        ramp.length(attackTime);
      }
      break;
      case DECAY:
        ramp.target(sustainLevelVal);
        ramp.length(decayTime);
      break;
      case SUSTAIN:
        ramp.value(sustainLevelVal);
      break;
      case RELEASE:
        ramp.target(0);
        ramp.length(releaseTime);
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
