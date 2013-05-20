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
  
  Tonic_::BufferFiller_::BufferFiller_() :  bufferReadPosition_(0) {
    setIsStereoOutput(true);
  }
    
}