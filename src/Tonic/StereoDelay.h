//
//  StereoDelay.h
//  Tonic 
//
//  Created by Morgan Packard on 3/13/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_STEREODELAY_H
#define TONIC_STEREODELAY_H

#include "Effect.h"
#include "DelayUtils.h"

namespace Tonic {
  
  namespace Tonic_ {

    class StereoDelay_ : public WetDryEffect_{
      
    protected:
    
      Generator delayTimeGen_[2];
      TonicFrames delayTimeFrames_[2];
      
      Generator fbkGen_;
      TonicFrames fbkFrames_;
      
      DelayLine delayLine_[2];
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    public:
      
      StereoDelay_();
      
      void initialize(float leftDelayArg, float rightDelayArg, float maxDelayLeft, float maxDelayRight);
      
      void setFeedback(Generator arg){
        fbkGen_ = arg;
      };
      
      void setDelayTimeLeft(Generator arg){
        delayTimeGen_[0] = arg;
      };
      
      void setDelayTimeRight(Generator arg){
        delayTimeGen_[1] = arg;
      };
      
      
    };
    
    inline void StereoDelay_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      delayTimeGen_[0].tick(delayTimeFrames_[TONIC_LEFT], context);
      delayTimeGen_[1].tick(delayTimeFrames_[TONIC_RIGHT], context);
      fbkGen_.tick(fbkFrames_, context);
      
      TonicFloat outSamp[2], fbk;
      TonicFloat *dryptr = &dryFrames_[0];
      TonicFloat *outptr = &outputFrames_[0];
      TonicFloat *fbkptr = &fbkFrames_[0];
      TonicFloat *delptr_l = &(delayTimeFrames_[TONIC_LEFT])[0];
      TonicFloat *delptr_r = &(delayTimeFrames_[TONIC_RIGHT])[0];
      
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
        
        // Don't clamp feeback - be careful! Negative feedback could be interesting.
        fbk = *fbkptr++;

        outSamp[TONIC_LEFT] = delayLine_[TONIC_LEFT].tickOut(*delptr_l++);
        outSamp[TONIC_RIGHT] = delayLine_[TONIC_RIGHT].tickOut(*delptr_r++);
        
        // output left sample
        *outptr++ = outSamp[TONIC_LEFT];
        delayLine_[TONIC_LEFT].tickIn(*dryptr++ + outSamp[TONIC_LEFT] * fbk);
        
        // output right sample
        *outptr++ = outSamp[TONIC_RIGHT];
        delayLine_[TONIC_RIGHT].tickIn(*dryptr++ + outSamp[TONIC_RIGHT] * fbk);
        
        // advance delay lines
        delayLine_[TONIC_LEFT].advance();
        delayLine_[TONIC_RIGHT].advance();
      }
    }
    
  }
  
  class StereoDelay : public TemplatedWetDryEffect<StereoDelay, Tonic_::StereoDelay_>{
    
  public:
  
  StereoDelay(float leftDelay, float rightDelay, float maxDelayLeft = -1, float maxDelayRight = -1);
  
  TONIC_MAKE_GEN_SETTERS(StereoDelay, feedback, setFeedback)
  TONIC_MAKE_GEN_SETTERS(StereoDelay, delayTimeLeft, setDelayTimeLeft)
  TONIC_MAKE_GEN_SETTERS(StereoDelay, delayTimeRight, setDelayTimeRight)

  };
}

#endif


