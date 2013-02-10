//
//  LFNoise.cpp
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

#include "LFNoise.h"
#include <algorithm>
#include "Util.h"

namespace Tonic {
namespace Tonic_{


LFNoise_::LFNoise_() : mCounter(0){
  mFreqFrames.resize(kSynthesisBlockSize);
}

void LFNoise_::tick(TonicFrames& frames){
  int remain = frames.frames();
  TonicFloat* out = &frames[0];
  do{
    if (mCounter<=0) {
      mFreq.tick(mFreqFrames);
			mCounter = sampleRate() / std::max<float>(mFreqFrames(0,0), .001f);
			mCounter = std::max<float>(1, mCounter);
			float nextlevel = Util::randFloat(-1, 1);
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

}
