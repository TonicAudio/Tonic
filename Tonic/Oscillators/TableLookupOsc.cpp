//
//  TableLookupOsc.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/8/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "TableLookupOsc.h"

namespace Tonic { namespace Tonic_{
  
  TableLookupOsc_::TableLookupOsc_() :
    time_(0.0), rate_(1.0)
  {
    modFrames.resize(kSynthesisBlockSize, 1);
  }
  
  TableLookupOsc_::~TableLookupOsc_(){
  
  }
  
  void TableLookupOsc_::reset(){
    time_ = 0.0f;
  }
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
