//
//  Divider.cpp
//  Tonic
//
//  Created by Morgan Packard on 4/19/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "Divider.h"

namespace Tonic { namespace Tonic_{
  
  Divider_::Divider_(){
    workSpace.resize(kSynthesisBlockSize, 1, 0);
  }
  
  Divider_::~Divider_(){
    
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
