//
//  ADSR.cpp
//  Tonic
//
//  Created by Morgan Packard on 2/28/13.

//

#include "ADSR.h"

namespace Tonic { namespace Tonic_{
  
  ADSR_::ADSR_() :
    state(ATTACK),
    lastValue(0),
    targetValue(0),
    increment(0),
    segCounter(0),
    segLength(0)
  {
    ramp.length(1.0);
    ramp.target(0);
    ramp.value(0);
    isLegato = ControlValue(false);
    doesSustain = ControlValue(true);
  }
  
  ADSR_::~ADSR_(){
    
  }
  
  void ADSR_::switchState(ADSRState newState){

    state = newState;
    segCounter = 0;

    switch(state){
        
      case NEUTRAL:{  
      
        lastValue = 0.f;
        increment = 0.f;
        
      }
      break;
        
      case ATTACK:{
        
        
        if (!bIsLegato){
          lastValue = 0.f;
        }
        
        segLength = attackTime * sampleRate();
        if (segLength == 0){
          switchState(DECAY);
        }
        else{
          targetValue = 1.0f;
          increment = (TonicFloat)(1.0f - lastValue)/segLength;
        }

      }
      break;
        
      case DECAY:{
        
        segLength = decayTime * sampleRate();
        if (segLength == 0){
          switchState(SUSTAIN);
        }
        else{
          targetValue = sustainLevelVal;
          increment = (TonicFloat)(sustainLevelVal - 1.0f)/segLength;
        }
        
      }
      break;
        
      case SUSTAIN:
      {
        lastValue = sustainLevelVal;
        increment = 0.f;
      }
      break;
        
      case RELEASE:{
        
        segLength = releaseTime * sampleRate();
        
        if (segLength == 0){
          switchState(NEUTRAL);
        }
        else{
          targetValue = 0.f;
          increment = (TonicFloat)(-lastValue)/segLength;
        }
      }
      break;
      
      default:
      break;
    }
  }
  
  
} // Namespace Tonic_

  
  ADSR::ADSR(float attackArg, float decayArg, float sustainArg, float releaseArg){
    attack(attackArg);
    decay(decayArg);
    sustain(sustainArg);
    release(releaseArg);
  }

  
} // Namespace Tonic
