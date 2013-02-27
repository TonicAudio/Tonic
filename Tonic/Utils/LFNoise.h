//
//  LFNoise.h
//  TonicDemo
//
//  Created by Morgan Packard on 1/29/13.
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

#ifndef __TonicDemo__LFNoise__
#define __TonicDemo__LFNoise__

#include <iostream>
#include "Generator.h"
#include "FixedValue.h"

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

