//
//  SawtoothWave.cpp
//  Tonic
//
//  Created by Nick Donaldson on 3/2/13.

//

#include "SawtoothWave.h"

namespace Tonic { namespace Tonic_{
  
  AngularWave_::AngularWave_() : phaseAccum_(0) {
    
    freqFrames_.resize(kSynthesisBlockSize, 1, 0);
    slopeFrames_.resize(kSynthesisBlockSize, 1, 0);
    
    slopeGen_ = FixedValue(0);
    freqGen_ = FixedValue(440);
    
  }
  
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
