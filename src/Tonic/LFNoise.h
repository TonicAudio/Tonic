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
      TonicFrames   mFreqFrames;
      float         mSlope;
      int           mCounter;
      float         mLevel;
      
      public:
      
        ControlGenerator     mFreq;
        void setFreq(ControlGenerator freq);
        void tick(TonicFrames& frames);
        void computeSynthesisBlock( const SynthesisContext_ &context );
        LFNoise_();
    
    };

  }
  
  class LFNoise : public TemplatedGenerator<Tonic_::LFNoise_>{
  public:
    createControlGeneratorSetters(LFNoise, setFreq, setFreq);
  };

}

#endif /* defined(__TonicDemo__LFNoise__) */

