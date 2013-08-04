//
//  SampleTables.cpp
//  Tonic
//
//  Created by Nick Donaldson on 5/17/13.
//

#include "SampleTable.h"

namespace Tonic {
  
  namespace Tonic_ {
    
    SampleTable_::SampleTable_(unsigned int frames, unsigned int channels){
      frames_.resize(frames, min(channels, 2)); // limited to 2 channels
    }
    
  }
}