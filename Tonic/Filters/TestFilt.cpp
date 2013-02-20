//
//  TestFilt.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/20/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "TestFilt.h"

namespace Tonic { namespace Tonic_{
  
  TestFilt_::TestFilt_(){
    // These represent a -12dB/oct Butterworth LPF with a Q of 2.0
    setCoefficients(0.0017876724793128818, 0.0035753449586257637, 0.0017876724793128818, -1.9510507971806847, 0.9582014870979363);
  }
  
  TestFilt_::~TestFilt_(){
    
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
