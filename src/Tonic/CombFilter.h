//
//  CombFilter.h
//  Tonic 
//
//  Created by Nick Donaldson on 5/4/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
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

#ifndef __Tonic__CombFilter__
#define __Tonic__CombFilter__

#include "Effect.h"
#include "DelayUtils.h"
#include "FilterUtils.h"

namespace Tonic {
  
  namespace Tonic_ {
    
    //! Absrtract comb-filter gen_ base class
    class CombFilter_ : public Effect_{
    
    protected:
      
      DelayLine   delayLine_;
      Generator   delayTimeGen_;
      Generator   scaleFactorGen_;
      
      TonicFrames delayTimeFrames_;
      TonicFrames scaleFactorFrames_;
      
    public:
      
      CombFilter_();
      
      void initialize(float initialDelayTime, float maxDelayTime);
      
      void setDelayTimeGen(Generator & gen){ delayTimeGen_ = gen; };
      
      void setScaleFactorGen(Generator & gen){ scaleFactorGen_ = gen; };
                  
    };
    
    //! Basic mono feed-forward comb filter
    class FFCombFilter_ : public CombFilter_{

    public:
      
      inline void computeSynthesisBlock( const SynthesisContext_ &context ){
        
        // tick modulations
        delayTimeGen_.tick(delayTimeFrames_, context);
        scaleFactorGen_.tick(scaleFactorFrames_, context);
        
        TonicFloat * inptr = &dryFrames_[0];
        TonicFloat * outptr = &synthesisBlock_[0];
        TonicFloat * dtptr = &delayTimeFrames_[0];
        TonicFloat * scptr = &scaleFactorFrames_[0];
        
        for (unsigned int i=0; i<kSynthesisBlockSize; i++){
          delayLine_.tickIn(*inptr);
          *outptr++ = (*inptr++ + delayLine_.tickOut(*dtptr++) * (*scptr)) * (1.0f/(1.0f + *scptr++));
          delayLine_.advance();
        }
      }
      
            
    };
    
    //! Basic mono feedback comb filter with hook for aditional processing pre-feedback
    class FBCombFilter_ : public CombFilter_{
      
    protected:
      
      // Does nothing by default, override to process post-delay, pre-sum
      virtual TonicFloat processFBSample(TonicFloat sample);
      
    public:

      inline void computeSynthesisBlock( const SynthesisContext_ &context ){
        
        // tick modulations
        delayTimeGen_.tick(delayTimeFrames_, context);
        scaleFactorGen_.tick(scaleFactorFrames_, context);
        
        TonicFloat y = 0;
        TonicFloat * inptr = &dryFrames_[0];
        TonicFloat * outptr = &synthesisBlock_[0];
        TonicFloat * dtptr = &delayTimeFrames_[0];
        TonicFloat * scptr = &scaleFactorFrames_[0];
        
        
        for (unsigned int i=0; i<kSynthesisBlockSize; i++){
          y = (processFBSample(delayLine_.tickOut(*dtptr++) * (*scptr)) + *inptr++) * (1.0f/(1.0f + *scptr++));
          delayLine_.tickIn(y);
          *outptr++ = y;
          delayLine_.advance();
        }
        
      }
      
    };
        
    inline TonicFloat FBCombFilter_::processFBSample(TonicFloat sample)
    {
      return sample;
    }

  }
  
  // -----------------------
  
  class FFCombFilter : public TemplatedEffect<FFCombFilter, Tonic_::FFCombFilter_>{

  public:
    
    FFCombFilter(float initialDelayTime = 0.1f, float maxDelayTime = -1);
    createGeneratorSetters(FFCombFilter, delayTime, setDelayTimeGen);
    createGeneratorSetters(FFCombFilter, scaleFactor, setScaleFactorGen);
  };
  
  // -----
  
  class FBCombFilter : public TemplatedEffect<FBCombFilter, Tonic_::FBCombFilter_>{
    
  public:
    
    FBCombFilter(float initialDelayTime = 0.1f, float maxDelayTime = -1);
    createGeneratorSetters(FBCombFilter, delayTime, setDelayTimeGen);
    createGeneratorSetters(FBCombFilter, scaleFactor, setScaleFactorGen);
  };
  
  // ------
}

#endif /* defined(__Tonic__CombFilter__) */


