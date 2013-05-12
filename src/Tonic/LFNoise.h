//
//  LFNoise.h
//  TonicDemo
//
//  Created by Morgan Packard on 1/29/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef __TonicDemo__LFNoise__
#define __TonicDemo__LFNoise__

#include <iostream>
#include "Generator.h"

namespace Tonic{

  namespace Tonic_{

    class LFNoise_ : public Generator_{
      
    protected:
      TonicFrames   mFreqFrames;
      float         mSlope;
      int           mCounter;
      float         mLevel;
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    public:
      
        ControlGenerator     mFreq;
        void setFreq(ControlGenerator freq);
        LFNoise_();
    
    };
    
    inline void LFNoise_::computeSynthesisBlock( const SynthesisContext_ &context ){
      int remain = outputFrames_.frames();
      TonicFloat* out = &outputFrames_[0];
      do{
        if (mCounter<=0) {
          mCounter = sampleRate() / std::max<float>(mFreq.tick(context).value, .001f);
          mCounter = std::max<float>(1, mCounter);
          float nextlevel = randomFloat(-1, 1);
          mSlope = (nextlevel - mLevel) / mCounter;
		}
        int nsmps = std::min(remain, mCounter);
        remain -= nsmps;
        mCounter -= nsmps;
        for(int i = 0; i < nsmps; i++){
          *(out++) = mLevel;
          mLevel += mSlope;
        }
      }while (remain);
    }


  }
  
  class LFNoise : public TemplatedGenerator<Tonic_::LFNoise_>{
  public:
    createControlGeneratorSetters(LFNoise, setFreq, setFreq);
  };

}

#endif /* defined(__TonicDemo__LFNoise__) */

