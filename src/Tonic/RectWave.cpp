//
//  RectWave.cpp
//  Tonic
//
//  Created by Nick Donaldson on 3/2/13.

//

#include "RectWave.h"

namespace Tonic { namespace Tonic_{
  
  RectWave_::RectWave_() : phaseAccum_(0) {
    pwmGen_ = FixedValue(0.5);
    freqFrames_.resize(kSynthesisBlockSize, 1 ,0);
    pwmFrames_.resize(kSynthesisBlockSize, 1, 0);
  }
  
  // ------
  
  RectWaveBL_::RectWaveBL_()
  {
    pwmGen_ = FixedValue(0.5);
    pwmFrames_.resize(kSynthesisBlockSize, 1, 0);
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
