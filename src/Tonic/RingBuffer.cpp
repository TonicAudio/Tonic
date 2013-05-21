//
//  RingBuffer.cpp
//  Tonic
//
//  Created by Nick Donaldson on 5/20/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "RingBuffer.h"

namespace Tonic {
  
  namespace Tonic_ {
    
    RingBuffer_::RingBuffer_() : ringBufferTable_(RingBufferTable(2,2)) {}
    
    RingBufferTable_::RingBufferTable_(unsigned int frames, unsigned int channels) :
    writeHead_(0),
    readHead_(0)
    {
      frames_.resize(frames, channels);
    }
    
  }
  
  RingBuffer & RingBuffer::initialize(string name, unsigned int nFrames, unsigned int nChannels){

    RingBufferTable bufferTable;
//    if (!SharedSampleTables().getSampleTable(name, &bufferTable)){
//      
//      RingBufferTable newTable = RingBufferTable(nFrames, nChannels);
//      SharedSampleTables().registerSampleTable(name, newTable);
//      
//      gen()->setRingBufferTable(newTable);
//    }
//    else{
//      gen()->setRingBufferTable(bufferTable);
//    }
    return *this;
  }
  
} // Namespace Tonic
