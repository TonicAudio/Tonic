//
//  SampleTables.cpp
//  TonicLib
//
//  Created by Nick Donaldson on 5/17/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "SampleTable.h"

namespace Tonic {
  
  namespace Tonic_ {
    
    SampleTable_::SampleTable_(unsigned int frames, unsigned int channels){
      frames_.resize(frames, min(channels, 2)); // limited to 2 channels
    }
    
  }
}