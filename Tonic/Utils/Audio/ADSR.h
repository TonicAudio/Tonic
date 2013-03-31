//
//  ADSR.h
//  Tonic 
//
//  Created by Morgan Packard on 2/28/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ADSR__
#define __Tonic__ADSR__

#include <iostream>
#include "Generator.h"
#include "ControlGenerator.h"
#include "RampedValue.h"

namespace Tonic {
  
  namespace Tonic_ {
  
    class ADSR_ : public Generator_{
      
    protected:
    
    
      RampedValue ramp;
      ControlGenerator mTrigger;
      ControlGenerator attack;
      ControlGenerator decay;
      ControlGenerator sustain;
      ControlGenerator release;
      ControlGenerator sustains;
      ControlGenerator isLegato;
      
      enum State{
        ATTACK,
        SUSTAIN,
        DECAY,
        RELEASE
      };
      
      State state;
      void switchState(State newState, const SynthesisContext_ &context);
            
    public:
      ADSR_();
      ~ADSR_();
      void computeSynthesisBlock( const SynthesisContext_ &context );
      void setTrigger(ControlGenerator trig){mTrigger = trig;}
      void setAttack(ControlGenerator gen){attack = gen;}
      void setDecay(ControlGenerator gen){decay = gen;}
      void setSustain(ControlGenerator gen){sustain = gen;}
      void setRelease(ControlGenerator gen){release = gen;}
      void setIsLegato(ControlGenerator gen){isLegato = gen;}
      
      //! Controls whether or not the envelope pauses on the SUSTAIN stage
      void setSustains(ControlGenerator gen){sustains = gen;};
      
    };
    
    inline void ADSR_::computeSynthesisBlock(const SynthesisContext_ &context){
      ControlGeneratorOutput triggerOutput = mTrigger.tick(context);
      
      // did a trigger message happen?
      if(triggerOutput.status == ControlGeneratorStatusHasChanged){
        if(triggerOutput.value != 0){
          switchState(ATTACK, context);
        }else if(sustains.tick(context).value != 0){
          switchState(RELEASE, context);
        }
        
      }else{
      
        switch (state) {
          case ATTACK:
            if(ramp.isFinished()){
              switchState(DECAY, context);
            }
            break;
            
          case DECAY:
            if(ramp.isFinished()){
              switchState(sustains.tick(context).value != 0 ? SUSTAIN : RELEASE, context);
            }
            break;

          default:
            break;
        }
      }
      ramp.tick(synthesisBlock_, context);
    }
    
  }
  
  
  /*!
    Classic ADSR envlelope. Non-zero trigger values correspond to key down. Trigger values of zero correspond to keyup.
    Time values are rounded up to the nearest buffer size.
    Time values are in milliseconds. 
  */
  
  class ADSR : public TemplatedGenerator<Tonic_::ADSR_>{
    
    public:
      ADSR();
      ADSR(float attack, float decay, float sustain, float release);
      createControlGeneratorSetters(ADSR, trigger, setTrigger);
      createControlGeneratorSetters(ADSR, attack, setAttack);
      createControlGeneratorSetters(ADSR, decay, setDecay);
      createControlGeneratorSetters(ADSR, sustain, setSustain);
      createControlGeneratorSetters(ADSR, release, setRelease);
      createControlGeneratorSetters(ADSR, doesSustain, setSustains);
      createControlGeneratorSetters(ADSR, legato, setIsLegato);

  };
  
}

#endif /* defined(__Tonic__ADSR__) */


