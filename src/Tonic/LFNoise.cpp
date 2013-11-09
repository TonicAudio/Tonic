//
//  LFNoise.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 1/29/13.

//
// See LICENSE.txt for license and usage information.
//


#include "LFNoise.h"

namespace Tonic {
namespace Tonic_{


LFNoise_::LFNoise_() : mCounter(0){
  mFreqFrames.resize(kSynthesisBlockSize);
}

void  LFNoise_::setFreq(ControlGenerator freq){
  mFreq = freq;
}

  
}

}
