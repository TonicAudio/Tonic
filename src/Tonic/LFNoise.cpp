//
//  LFNoise.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 1/29/13.

//
// See LICENSE.txt for license and usage information.
//


#include "LFNoise.h"
#ifndef _WIN32
#include "Util.h"
#endif
namespace Tonic {
namespace Tonic_{


LFNoise_::LFNoise_() : mCounter(0){
  mFreqFrames.resize(kSynthesisBlockSize);
}

void  LFNoise_::setFreq(ControlGenerator freq){
  mFreq = freq;
}

void LFNoise_::computeSynthesisBlock( const SynthesisContext_ &context ){
  int remain = synthesisBlock_.frames();
  TonicFloat* out = &synthesisBlock_[0];
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

}
