//
//  StereoDelay.h
//  Tonic 
//
//  Created by Morgan Packard on 3/13/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__StereoDelay__
#define __Tonic__StereoDelay__

#include "Effect.h"
#include "DelayUtils.h"

namespace Tonic {
  
  namespace Tonic_ {

    class StereoDelay_ : public Effect_{
      
    protected:
    
      Generator delayTimeGen_[2];
      TonicFrames delayTimeFrames_[2];
      
      Generator mixGen_;
      TonicFrames mixFrames_;
      
      Generator fbkGen_;
      TonicFrames fbkFrames_;
      
      DelayLine delayLine_[2];
            
    public:
      
      StereoDelay_();
      ~StereoDelay_();
      
      void initialize(float leftDelayArg, float rightDelayArg, float maxDelayLeft, float maxDelayRight);
      
      void setMix(Generator & arg){
        mixGen_ = arg;
      };
      
      void setFeedback(Generator & arg){
        fbkGen_ = arg;
      };
      
      void setDelayTimeLeft(Generator & arg){
        delayTimeGen_[0] = arg;
      };
      
      void setDelayTimeRight(Generator & arg){
        delayTimeGen_[1] = arg;
      };
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    };
    
    inline void StereoDelay_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      delayTimeGen_[0].tick(delayTimeFrames_[0], context);
      delayTimeGen_[1].tick(delayTimeFrames_[1], context);
      mixGen_.tick(mixFrames_, context);
      fbkGen_.tick(fbkFrames_, context);
      
      TonicFloat outSamp[2], mix, fbk;
      TonicFloat *dryptr = &dryFrames_[0];
      TonicFloat *outptr = &outputFrames_[0];
      TonicFloat *mptr = &mixFrames_[0];
      TonicFloat *fbkptr = &fbkFrames_[0];
      TonicFloat *delptr_l = &(delayTimeFrames_[0])[0];
      TonicFloat *delptr_r = &(delayTimeFrames_[1])[0];
      
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
        
        // Don't clamp feeback - be careful! Negative feedback could be interesting.
        fbk = *fbkptr++;
        mix = clamp(*mptr++, 0.0f, 1.0f);

        outSamp[0] = delayLine_[0].tickOut();
        outSamp[1] = delayLine_[1].tickOut();
        
        // output left sample
        *outptr++ = (*dryptr * (1.0f - mix)) + (outSamp[0] * mix);
        delayLine_[0].tickIn(*dryptr++ + outSamp[0] * fbk);
        
        // output right sample
        *outptr++ = (*dryptr * (1.0f - mix)) + (outSamp[1] * mix);
        delayLine_[1].tickIn(*dryptr++ + outSamp[1] * fbk);
        
        // advance delay lines
        delayLine_[0].advance(*delptr_l++);
        delayLine_[1].advance(*delptr_r++);
      }
    }
    
  }
  
  class StereoDelay : public TemplatedEffect<StereoDelay, Tonic_::StereoDelay_>{
    
  public:
  
  StereoDelay(float leftDelay, float rightDelay, float maxDelayLeft = -1, float maxDelayRight = -1);
  
  createGeneratorSetters(StereoDelay, mix, setMix)
  createGeneratorSetters(StereoDelay, feedback, setFeedback)
  createGeneratorSetters(StereoDelay, delayTimeLeft, setDelayTimeLeft)
  createGeneratorSetters(StereoDelay, delayTimeRight, setDelayTimeRight)

  };
}

#endif /* defined(__Tonic__StereoDelay__) */


