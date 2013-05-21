//
//  BufferFiller.cpp
//  TonicDemo
//
//  Created by Nick Donaldson on 2/9/13.

//
// See LICENSE.txt for license and usage information.
//


#include "BufferFiller.h"

namespace Tonic {
  
  namespace Tonic_{
    
    BufferFiller_::BufferFiller_() :  bufferReadPosition_(0) {
      TONIC_MUTEX_INIT(mutex_);
      setIsStereoOutput(true);
    }
    
    BufferFiller_::~BufferFiller_(){
      TONIC_MUTEX_DESTROY(mutex_);
    }
    
  }
}