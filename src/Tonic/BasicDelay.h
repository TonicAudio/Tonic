//
//  BasicDelay.h
//  Tonic 
//
//  Created by Nick Donaldson on 3/10/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__BasicDelay__
#define __Tonic__BasicDelay__

#include "Effect.h"
#include "DelayUtils.h"

namespace Tonic {
  
  namespace Tonic_ {

    class BasicDelay_ : public Effect_{
      
    protected:
      
      Generator delayTimeGen_;
      TonicFrames delayTimeFrames_;
      
      Generator mixGen_;
      TonicFrames mixFrames_;
      
      Generator fbkGen_;
      TonicFrames fbkFrames_;

      DelayLine delayLine_;
            
    public:
      
      BasicDelay_();
      ~BasicDelay_();
      
      // Overridden so output channel layout follows input channel layout
      void setInput( Generator input );
      
      void initialize(float delayTime, float maxDelayTime);
      
      void setDelayTimeGen( Generator & gen ) { delayTimeGen_ = gen; };
      
      void setMixGen( Generator & gen ) { mixGen_ = gen; };
      
      void setFeedbackGen( Generator & gen ) { fbkGen_ = gen; };
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    };
    
    inline void BasicDelay_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      delayTimeGen_.tick(delayTimeFrames_, context);
      mixGen_.tick(mixFrames_, context);
      fbkGen_.tick(fbkFrames_, context);
      
      // input->output always has same channel layout
      unsigned int nChannels = isStereoInput() ? 2 : 1;
      
      TonicFloat outSamp, mix, fbk;
      TonicFloat *dryptr = &dryFrames_[0];
      TonicFloat *outptr = &outputFrames_[0];
      TonicFloat *mptr = &mixFrames_[0];
      TonicFloat *fbkptr = &fbkFrames_[0];
      TonicFloat *delptr = &delayTimeFrames_[0];
      
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
        
        // Don't clamp feeback - be careful! Negative feedback could be interesting.
        fbk = *fbkptr++;
        mix = clamp(*mptr++, 0.0f, 1.0f);
        
        for (unsigned int c=0; c<nChannels; c++){
          outSamp = delayLine_.tickOut(c);
          *outptr++ = (*dryptr * (1.0f - mix)) + (outSamp * mix);
          delayLine_.tickIn(*dryptr++ + outSamp * fbk, c);
        }
        
        delayLine_.advance(*delptr++);
      }
    }
    
  }
  
  
  class BasicDelay : public TemplatedEffect<BasicDelay, Tonic_::BasicDelay_>{
    
  public:
    //! Allocating only with time argument will default max delay time to 1.5 * delayTime
    BasicDelay(float initialDelayTime = 0.5f, float maxDelayTime = -1);
    
    createGeneratorSetters(BasicDelay, delayTime, setDelayTimeGen);
    
    //! Warning: Feedback input is NOT clamped! Beware of feedback values greater than 1 !!!
    createGeneratorSetters(BasicDelay, feedback, setFeedbackGen);

    createGeneratorSetters(BasicDelay, mix, setMixGen);
  };
}

#endif /* defined(__Tonic__BasicDelay__) */


