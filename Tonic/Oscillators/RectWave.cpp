//
//  RectWave.cpp
//  Tonic
//
//  Created by Nick Donaldson on 3/2/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "RectWave.h"

namespace Tonic { namespace Tonic_{
  
  RectWave_::RectWave_(){
    freqFrames_.resize(kSynthesisBlockSize, 1 ,0);
    pwmFrames_.resize(kSynthesisBlockSize, 1, 0);
  }
  
  RectWave_::~RectWave_(){
    
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
