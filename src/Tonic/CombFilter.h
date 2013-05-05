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

    //! Basic mono feed-forward comb filter with post-delay hook for aditional processing
    class FFCombFilter_ : public Effect_{
      
    protected:
      
      DelayLine   delayLine_;
      Generator   delayTimeGen_;
      Generator   ffAmountGen_;
      
      TonicFrames delayTimeFrames_;
      TonicFrames ffAmountFrames_;
      
      // Does nothing by default, override to process post-delay, pre-sum
      virtual TonicFloat processFFSample(TonicFloat sample);

    public:
      
      FFCombFilter_();
      ~FFCombFilter_();
      
      void initialize(float initialDelayTime, float maxDelayTime);
      
      void setDelayTimeGen(Generator & gen){ delayTimeGen_ = gen; };
      
      void setFFAmountGen(Generator & gen){ ffAmountGen_ = gen; };
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
            
    };
    
    inline void FFCombFilter_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      
    }
    
    inline TonicFloat FFCombFilter_::processFFSample(TonicFloat sample){
      return sample;
    }

  }
  
  class FFCombFilter : public TemplatedEffect<FFCombFilter, Tonic_::FFCombFilter_>{

  public:
    
    FFCombFilter(float initialDelayTime = 0.1f, float maxDelayTime = -1);

  };
}

#endif /* defined(__Tonic__CombFilter__) */


