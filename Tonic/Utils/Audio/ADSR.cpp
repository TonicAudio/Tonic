//
//  ADSR.cpp
//  Tonic
//
//  Created by Morgan Packard on 2/28/13.

//

#include "ADSR.h"

namespace Tonic { namespace Tonic_{
  
  ADSR_::ADSR_() :
    state(NEUTRAL),
    lastValue(0),
    targetValue(0),
    increment(0),
    segCounter(0),
    segLength(0),
    pole(0)
  {
    mTrigger = ControlValue(0); // empty trigger by default
    isLegato = ControlValue(false);
    doesSustain = ControlValue(true);
    isExponential = ControlValue(false);
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
        pole = t60ToTau(attackTime);
        
        if (segLength == 0){
          lastValue = 1.0f;
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
        pole = t60ToTau(decayTime);
        
        if (segLength == 0){
          switchState(SUSTAIN);
        }
        else{
          targetValue = sustainLevelVal;
          increment = (TonicFloat)(sustainLevelVal - lastValue)/segLength;
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
        pole = t60ToTau(releaseTime);
        
        if (segLength == 0){
          lastValue = 0.f;
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
