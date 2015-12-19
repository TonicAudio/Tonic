//
//  ADSR.h
//  Tonic 
//
//  Created by Morgan Packard on 2/28/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_ADSR_H
#define TONIC_ADSR_H

#include "Generator.h"
#include "ControlGenerator.h"
#include "FilterUtils.h"

namespace Tonic {
  
  namespace Tonic_ {
  
    class ADSR_ : public Generator_{
      
    protected:
    
    
      ControlGenerator mTrigger;
      ControlGenerator attack;
      ControlGenerator decay;
      ControlGenerator sustain;
      ControlGenerator release;
      ControlGenerator doesSustain;
      ControlGenerator isLegato;
      ControlGenerator isExponential;
      
      TonicFloat attackTime;
      TonicFloat decayTime;
      TonicFloat sustainLevelVal;
      TonicFloat releaseTime;
      bool       bIsLegato;
      bool       bDoesSustain;
      bool       bIsExponential;
      
      // state variables
      unsigned long segCounter;
      unsigned long segLength;
      TonicFloat targetValue;
      TonicFloat lastValue;
      TonicFloat increment;
      TonicFloat pole;
      
      enum ADSRState {
        NEUTRAL,
        ATTACK,
        SUSTAIN,
        DECAY,
        RELEASE
      };
      
      ADSRState state;
      void switchState(ADSRState newState);
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    public:
      
      ADSR_();
      ~ADSR_();
            
      void setTrigger(ControlGenerator trig){mTrigger = trig;}
      void setAttack(ControlGenerator gen){attack = gen;}
      void setDecay(ControlGenerator gen){decay = gen;}
      void setSustain(ControlGenerator gen){sustain = gen;}
      void setRelease(ControlGenerator gen){release = gen;}
      
      //! Exponential or linear ramp
      void setIsExponential(ControlGenerator gen){isExponential = gen;}
      
      //! Controls whether the envelope picks up from current position or zero when re-triggered while still releasing
      void setIsLegato(ControlGenerator gen){isLegato = gen;}
      
      //! Controls whether or not the envelope pauses on the SUSTAIN stage
      void setDoesSustain(ControlGenerator gen){doesSustain = gen;};
      
    };
    
    inline void ADSR_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      ControlGeneratorOutput triggerOutput = mTrigger.tick(context);
      
      // frames to go in this block
      
      // Tick ALL inputs every time to keep everything in sync
      attackTime = attack.tick(context).value;
      decayTime = decay.tick(context).value;
      sustainLevelVal = sustain.tick(context).value;
      releaseTime = release.tick(context).value;
      bIsExponential = (bool)isExponential.tick(context).value;
      bDoesSustain = (bool)doesSustain.tick(context).value;
      bIsLegato = (bool)isLegato.tick(context).value;
      
      TonicFloat * fdata = &outputFrames_[0];
      
      if(triggerOutput.triggered){
        
        if(triggerOutput.value != 0){
          switchState(ATTACK);
        }else if(bDoesSustain){
          switchState(RELEASE);
        }
        
      }
      
      int samplesRemaining = kSynthesisBlockSize;
      
      while (samplesRemaining > 0)
      {
        switch (state) {
          
          // Both of these cases just fill the synthesis block the rest of the way up
          case NEUTRAL:
          case SUSTAIN:
          {
            #ifdef USE_APPLE_ACCELERATE
            vDSP_vfill(&lastValue, fdata, 1, samplesRemaining);
            #else
            std::fill(fdata, fdata + samplesRemaining, lastValue);
            #endif
            
            samplesRemaining = 0;
          }
            break;
            
          case ATTACK:
          case DECAY:
          case RELEASE:
          {
            
            // how many samples remain in current segment
            unsigned long remainder = (segCounter > segLength) ? 0 : segLength - segCounter;
            if (remainder <= samplesRemaining){
              
              // fill up part of the ramp then switch segment

              if (bIsExponential){

                // one pole filter
                for (unsigned long i=0; i<remainder; i++){
                  onePoleLPFTick(targetValue, lastValue, pole);
                  *fdata++ = lastValue;
                }
                
              }
              else{
                
                #ifdef USE_APPLE_ACCELERATE
                // starting point
                lastValue += increment;
                
                // vector calculation
                vDSP_vramp(&lastValue, &increment, fdata, 1, remainder);
                
                // end point
                lastValue += increment*(remainder-1);
                fdata += remainder;
                
                #else
                for (unsigned long i=0; i<remainder; i++){
                  lastValue += increment;
                  *fdata++ = lastValue;
                }
                #endif
              }
              
              segCounter += remainder;
              samplesRemaining -= remainder;
              
              // switch segment
              if (state == ATTACK){
                switchState(DECAY);
              }
              else if (state == DECAY){
                switchState(bDoesSustain ? SUSTAIN : RELEASE);
              }
              else{
                switchState(NEUTRAL);
              }
              
            }
            else{
              
              if (bIsExponential){
                
                // one pole filter
                for (int i=0; i<samplesRemaining; i++){
                  onePoleLPFTick(targetValue, lastValue, pole);
                  *fdata++ = lastValue;
                }
                
              }
              else{
                // fill the rest of the ramp up
                #ifdef USE_APPLE_ACCELERATE
                // starting point
                lastValue += increment;
                
                // vector calculation
                vDSP_vramp(&lastValue, &increment, fdata, 1, samplesRemaining);
                
                // end point
                lastValue += increment*(samplesRemaining-1);
                
                #else
                for (int i=0; i<samplesRemaining; i++){
                  lastValue += increment;
                  *fdata++ = lastValue;
                }
                #endif
                
              }
              
              segCounter += samplesRemaining;
              samplesRemaining = 0;
            }
          }
            break;
            
          default:
            break;
        }
        
        
      }
      
    }
    
  }
  
  
  /*!
    Classic ADSR envlelope. Non-zero trigger values correspond to key down. Trigger values of zero correspond to keyup.
    Time values are rounded up to the nearest buffer size.
    Time values are in milliseconds. 
  */
  
  class ADSR : public TemplatedGenerator<Tonic_::ADSR_>{
    
    public:
    
      ADSR(float attack = 0.001f, float decay = 0.03f, float sustain = 1.0f, float release = 0.05f);
    
      TONIC_MAKE_CTRL_GEN_SETTERS(ADSR, trigger, setTrigger);
      TONIC_MAKE_CTRL_GEN_SETTERS(ADSR, attack, setAttack);
      TONIC_MAKE_CTRL_GEN_SETTERS(ADSR, decay, setDecay);
      TONIC_MAKE_CTRL_GEN_SETTERS(ADSR, sustain, setSustain);
      TONIC_MAKE_CTRL_GEN_SETTERS(ADSR, release, setRelease);
      TONIC_MAKE_CTRL_GEN_SETTERS(ADSR, exponential, setIsExponential);
      TONIC_MAKE_CTRL_GEN_SETTERS(ADSR, doesSustain, setDoesSustain);
      TONIC_MAKE_CTRL_GEN_SETTERS(ADSR, legato, setIsLegato);

  };
  
}

#endif


