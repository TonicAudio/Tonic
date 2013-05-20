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
  
  BufferFiller::BufferFiller() : bufferReadPosition_(0){
    TONIC_MUTEX_INIT(&mutex_);
    outputFrames_.resize(kSynthesisBlockSize, 2);
  }
  
  BufferFiller::~BufferFiller(){
    TONIC_MUTEX_DESTROY(&mutex_);
  }
  
}