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
      
      DelayLine           delayLine_;
      Generator           delayTimeGen_;
      ControlGenerator    scaleFactorCtrlGen_;
      
      TonicFrames         delayTimeFrames_;
      
    public:
      
      CombFilter_();
      
      void initialize(float initialDelayTime, float maxDelayTime);
      
      void setDelayTimeGen(Generator gen){ delayTimeGen_ = gen; };
      
      void setScaleFactorGen(ControlGenerator gen){ scaleFactorCtrlGen_ = gen; };
                  
    };
    
    //! Basic mono feed-forward comb filter
    class FFCombFilter_ : public CombFilter_{

    protected:
      
      inline void computeSynthesisBlock( const SynthesisContext_ &context ){
        
        // tick modulations
        delayTimeGen_.tick(delayTimeFrames_, context);
        
        TonicFloat * inptr = &dryFrames_[0];
        TonicFloat * outptr = &outputFrames_[0];
        TonicFloat * dtptr = &delayTimeFrames_[0];
        TonicFloat sf = scaleFactorCtrlGen_.tick(context).value;
        TonicFloat norm = (1.0f/(1.0f + sf));
        
        for (unsigned int i=0; i<kSynthesisBlockSize; i++){
          delayLine_.tickIn(*inptr);
          *outptr++ = (*inptr++ + delayLine_.tickOut(*dtptr++) * sf) * norm;
          delayLine_.advance();
        }
      }
      
            
    };
    
    //! Basic mono feedback comb filter with hook for aditional processing pre-feedback
    class FBCombFilter_ : public CombFilter_{
      
    protected:

      inline void computeSynthesisBlock( const SynthesisContext_ &context ){
        
        // tick modulations
        delayTimeGen_.tick(delayTimeFrames_, context);
        
        TonicFloat y = 0;
        TonicFloat * inptr = &dryFrames_[0];
        TonicFloat * outptr = &outputFrames_[0];
        TonicFloat * dtptr = &delayTimeFrames_[0];
        TonicFloat sf = scaleFactorCtrlGen_.tick(context).value;
        TonicFloat norm = (1.0f/(1.0f + sf));
        
        for (unsigned int i=0; i<kSynthesisBlockSize; i++){
          y = ((delayLine_.tickOut(*dtptr++) * sf) + *inptr++) * norm;
          delayLine_.tickIn(y);
          *outptr++ = y;
          delayLine_.advance();
        }
        
      }
      
    };
  
    
    // ------------------------
    
    //! Feedback comb filter with 6dB/oct (one-pole) lpf and hpf, optimized for reverb
    class FilteredFBCombFilter6_ : public CombFilter_{
      
    protected:
      
      TonicFloat lastOutLow_;
      TonicFloat lastOutHigh_;
      
      ControlGenerator lowCutoffGen_;
      ControlGenerator highCutoffGen_;
      
      ControlGenerator scaleFactorGen_;
      
      void computeSynthesisBlock( const SynthesisContext_ &context );

      
    public:
      
      FilteredFBCombFilter6_();
      
      void setLowCutoff( ControlGenerator gen ) { lowCutoffGen_ = gen; };
      void setHighCutoff( ControlGenerator gen ) { highCutoffGen_ = gen; };
      
    };
    
    inline void FilteredFBCombFilter6_::computeSynthesisBlock( const SynthesisContext_ &context ){
      
      // tick modulations
      delayTimeGen_.tick(delayTimeFrames_, context);
      
      TonicFloat y = 0;
      TonicFloat * inptr = &dryFrames_[0];
      TonicFloat * outptr = &outputFrames_[0];
      TonicFloat * dtptr = &delayTimeFrames_[0];
      
      TonicFloat sf = scaleFactorCtrlGen_.tick(context).value;
      
      TonicFloat lowCoef = cutoffToOnePoleCoef(lowCutoffGen_.tick(context).value);
      TonicFloat hiCoef = 1.0f - cutoffToOnePoleCoef(highCutoffGen_.tick(context).value);
      
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
        onePoleLPFTick(delayLine_.tickOut(*dtptr++), lastOutLow_, lowCoef);
        onePoleHPFTick(lastOutLow_, lastOutHigh_, hiCoef);
        y = ((lastOutHigh_ * sf) + *inptr++); // no normalization on purpose
        delayLine_.tickIn(y);
        *outptr++ = y;
        delayLine_.advance();
      }
      
    }

  }
  
  // -----------------------
  
  // TODO: Could template-ify these for DRY-er subclassing...
  
  class FFCombFilter : public TemplatedEffect<FFCombFilter, Tonic_::FFCombFilter_>{

  public:
    
    FFCombFilter(float initialDelayTime = 0.1f, float maxDelayTime = -1);
    createGeneratorSetters(FFCombFilter, delayTime, setDelayTimeGen);
    createControlGeneratorSetters(FFCombFilter, scaleFactor, setScaleFactorGen);
  };
  
  // -----
  
  class FBCombFilter : public TemplatedEffect<FBCombFilter, Tonic_::FBCombFilter_>{
    
  public:
    
    FBCombFilter(float initialDelayTime = 0.1f, float maxDelayTime = -1);
    createGeneratorSetters(FBCombFilter, delayTime, setDelayTimeGen);
    createControlGeneratorSetters(FBCombFilter, scaleFactor, setScaleFactorGen);
  };
  
  // ------
  
  class FilteredFBCombFilter6 : public TemplatedEffect<FilteredFBCombFilter6, Tonic_::FilteredFBCombFilter6_>
  {
  public:
    FilteredFBCombFilter6(float initialDelayTime = 0.1f, float maxDelayTime = -1);
    createGeneratorSetters(FilteredFBCombFilter6, delayTime, setDelayTimeGen);
    createControlGeneratorSetters(FilteredFBCombFilter6, scaleFactor, setScaleFactorGen);
    createControlGeneratorSetters(FilteredFBCombFilter6, lowpassCutoff, setLowCutoff);
    createControlGeneratorSetters(FilteredFBCombFilter6, highpassCutoff, setHighCutoff);
  };
}

#endif /* defined(__Tonic__CombFilter__) */


