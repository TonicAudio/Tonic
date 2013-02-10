//
//  Noise.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/7/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "Noise.h"

namespace Tonic { namespace Tonic_{
  
  Noise_::Noise_() : stereo_(false) {}
  
  Noise_::~Noise_() {}
  
} // Namespace Tonic_
  
  Noise & Noise::stereo(bool stereo) {
    gen()->setStereo(stereo);
    return *this;
  }
  
  
} // Namespace Tonic
