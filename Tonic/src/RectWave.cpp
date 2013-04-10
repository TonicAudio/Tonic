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
  
  RectWave_::~RectWave_(){
    
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
