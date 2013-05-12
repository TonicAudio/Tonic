//
//  TableLookupOsc.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/8/13.

//

#include "TableLookupOsc.h"

namespace Tonic { namespace Tonic_{
  
  TableLookupOsc_::TableLookupOsc_() :
    phase_(0.0)
  {
    modFrames_.resize(kSynthesisBlockSize, 1);
  }
  
  void TableLookupOsc_::reset(){
    phase_ = 0.0f;
  }

} // Namespace Tonic_
  
  
  
} // Namespace Tonic
