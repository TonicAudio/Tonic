//
//  Biquad.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/20/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "Biquad.h"

namespace Tonic { namespace Tonic_{
  
  Biquad_::Biquad_(){
    memset(coef_, 0, 5 * sizeof(TonicFloat));
    inputVec_.resize(kSynthesisBlockSize + 2, 2, 0);
    outputVec_.resize(kSynthesisBlockSize + 2, 2, 0);
  }
  
  Biquad_::~Biquad_(){
    
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
