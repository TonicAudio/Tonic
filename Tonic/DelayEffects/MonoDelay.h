//
//  MonoDelay.h
//  Tonic 
//
//  Created by Nick Donaldson on 3/10/13.
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

#ifndef __Tonic__MonoDelay__
#define __Tonic__MonoDelay__

#include "Effect.h"
#include "DelayUtils.h"

namespace Tonic {
  
  namespace Tonic_ {

    class MonoDelay_ : public Effect_{
      
    protected:
      
      Generator delayTimeGen_;
      TonicFrames delayTimeFrames_;
      
      Generator mixGen_;
      TonicFrames mixFrames_;
      
      Generator fbkGen_;
      TonicFrames fbkFrames_;
      
      // Need to allocate manually to set fixed max delay length
      // without assinging over another instance, which would inefficiently
      // allocate DelayLine data twice.
      DelayLine* delayLine_;
      
    public:
      
      MonoDelay_();
      ~MonoDelay_();
      
      void initialize(float delayTime, float maxDelayTime);
      
      void setDelayTimeGen( Generator & gen ) { delayTimeGen_ = gen; };
      
      void setMixGen( Generator & gen ) { mixGen_ = gen; };
      
      void setFeedbackGen( Generator & gen ) { fbkGen_ = gen; };
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    };
    
    inline void MonoDelay_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      delayTimeGen_.tick(delayTimeFrames_, context);
      mixGen_.tick(mixFrames_, context);
      fbkGen_.tick(fbkFrames_, context);
      
      input_.tick(synthesisBlock_, context);
      
      TonicFloat inSamp, outSamp, mix;
      TonicFloat *dptr = &synthesisBlock_[0];
      TonicFloat *mptr = &mixFrames_[0];
      TonicFloat *fbkptr = &fbkFrames_[0];
      TonicFloat *delptr = &delayTimeFrames_[0];
      
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
        
        inSamp = *dptr;
        mix = clamp(*mptr++, 0.0f, 1.0f);
        outSamp = delayLine_->tickOut();
        *dptr++ = (inSamp * (1.0f - mix)) + (outSamp * mix);;
        
        // Don't clamp feeback - be careful! Negative feedback could be interesting.
        delayLine_->tickIn(inSamp + outSamp * (*fbkptr++));
        delayLine_->advance(*delptr++);
      }
    }
    
  }
  
  
  class MonoDelay : public TemplatedEffect<MonoDelay, Tonic_::MonoDelay_>{
    
  public:
    //! Allocating only with time argument will default max delay time to 1.5 * delayTime
    MonoDelay(float delayTime = 0.5f, float maxDelayTime = -1){
      gen()->initialize(delayTime, maxDelayTime);
    }
    
    createGeneratorSetters(MonoDelay, delayTime, setDelayTimeGen);
    
    //! Warning: Feedback input is NOT clamped! Beware of feedback values greater than 1 !!!
    createGeneratorSetters(MonoDelay, feedback, setFeedbackGen);

    createGeneratorSetters(MonoDelay, mix, setMixGen);
  };
}

#endif /* defined(__Tonic__MonoDelay__) */


