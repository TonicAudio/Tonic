//
//  ADSR.h
//  Tonic 
//
//  Created by Morgan Packard on 2/28/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

/*+++++++++++++++++++++ License ++++++++++++++++++++

Use this code for whatever you want. There are NO 
RESTRICTIONS WHATSOVER. Modify it, repackage it, 
sell it, get rich from it, whatever. Go crazy. If 
you want to make mehappy, contribute to this 
project, but feel free to just use the code as a 
starting point for whatever you like.

Note that Tonic is heavily indebted to STK
https://ccrma.stanford.edu/software/stk/

++++++++++++++++++++++++++++++++++++++++++++++++++*/

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
      
      TonicFloat attackTime;
      TonicFloat decayTime;
      TonicFloat sustainLevelVal;
      TonicFloat releaseTime;
      bool       legatoVal;
      bool       sustainsVal;
      
      enum State{
        ATTACK,
        SUSTAIN,
        DECAY,
        RELEASE
      };
      
      State state;
      void switchState(State newState);
            
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
      
      // Tick ALL inputs every time to keep everything in sync
      attackTime = attack.tick(context).value;
      decayTime = decay.tick(context).value;
      sustainLevelVal = sustain.tick(context).value;
      releaseTime = release.tick(context).value;
      sustainsVal = (bool)sustain.tick(context).value;
      legatoVal = (bool)isLegato.tick(context).value;
      
      // did a trigger message happen?
      if(triggerOutput.status == ControlGeneratorStatusHasChanged){
        
        if(triggerOutput.value != 0){
          switchState(ATTACK);
        }else if(sustainsVal != 0){
          switchState(RELEASE);
        }
        
      }
      else{
      
        switch (state) {
          case ATTACK:
            if(ramp.isFinished()){
              switchState(DECAY);
            }
            break;
            
          case DECAY:
            if(ramp.isFinished()){
              switchState(sustainsVal != 0 ? SUSTAIN : RELEASE);
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


