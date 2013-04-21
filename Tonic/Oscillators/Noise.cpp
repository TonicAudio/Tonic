//
//  Noise.h
//  Tonic
//
//  Created by Nick Donaldson on 2/7/13.

//
// See LICENSE.txt for license and usage information.
//


#include "Noise.h"

namespace Tonic {
  namespace Tonic_ {
  
    PinkNoise_::PinkNoise_() : pinkCount_(1), pinkAccum_(0) {
      memset(pinkBins_, 0, kNumPinkNoiseBins * sizeof(TonicFloat));
    }
  
  }
}