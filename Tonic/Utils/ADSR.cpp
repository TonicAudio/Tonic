//
//  ADSR.cpp
//  Tonic
//
//  Created by Morgan Packard on 2/28/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "ADSR.h"

namespace Tonic { namespace Tonic_{
  
  ADSR_::ADSR_(){
    decay.lengthMs(1000);
    decay.target(0);
    decay.value(0);
  }
  
  ADSR_::~ADSR_(){
    
  }
  
  void ADSR_::trigger(){
    decay.value(1);
    decay.target(0);
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
