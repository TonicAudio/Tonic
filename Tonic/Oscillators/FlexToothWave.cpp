//
//  FlexToothWave.cpp
//  Tonic
//
//  Created by Nick Donaldson on 3/2/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "FlexToothWave.h"

namespace Tonic { namespace Tonic_{
  
  FlexToothWave_::FlexToothWave_() : phaseAccum_(0) {
    
    freqFrames_.resize(kSynthesisBlockSize, 1, 0);
    slopeFrames_.resize(kSynthesisBlockSize, 1, 0);
    
  }
  
  FlexToothWave_::~FlexToothWave_(){
    
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
