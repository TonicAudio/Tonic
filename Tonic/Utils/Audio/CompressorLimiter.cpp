//
//  CompressorLimiter.cpp
//  Tonic
//
//  Created by Nick Donaldson on 3/24/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "CompressorLimiter.h"

namespace Tonic { namespace Tonic_{
  
  Compressor_::Compressor_(){
    setIsStereo(true);
    ampInputFrames_.resize(kSynthesisBlockSize, 2, 0);
    lookaheadDelayLine_.initialize(0.001, 0.01, 2);
    lookaheadDelayLine_.setInterpolates(false); // No real need to interpolate here for lookahead
  }
  
  Compressor_::~Compressor_(){
    
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
