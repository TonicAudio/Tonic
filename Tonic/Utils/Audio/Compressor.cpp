//
//  Compressor.cpp
//  Tonic
//
//  Created by Nick Donaldson on 3/24/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "Compressor.h"

namespace Tonic { namespace Tonic_{
  
  Compressor_::Compressor_(){
    setIsStereo(true);
    lookaheadDelayLine_.initialize(0.001, 0.01, 2);
  }
  
  Compressor_::~Compressor_(){
    
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
