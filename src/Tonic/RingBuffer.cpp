//
//  RingBuffer.cpp
//  Tonic
//
//  Created by Nick Donaldson on 5/20/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "RingBuffer.h"

namespace Tonic {
  
  //! Registry of ring buffers
  TonicDictionary<RingBufferTable> s_RingBuffers_;
  
  namespace Tonic_ {
    
    RingBufferTable_::RingBufferTable_(unsigned int frames, unsigned int channels) :
      SampleTable_(frames, channels),
      writeHead_(0),
      readHead_(0)
    {}
  }
  
  RingBufferReader & RingBufferReader::bufferName(string name){

    if (s_RingBuffers_.containsObjectNamed(name)){      
      gen()->setRingBufferTable(s_RingBuffers_.objectNamed(name));
    }
    else{
      error("RingBuffer named " + name + " does not exist.");
    }
    return *this;
  }

  RingBufferWriter::RingBufferWriter() {}
  
  RingBufferWriter::RingBufferWriter(string name, unsigned int nFrames, unsigned int nChannels)
  {
    ringBufferTable_ = RingBufferTable(nFrames, nChannels);
    
    // overwrite existing entry if there is one
#ifdef TONIC_DEBUG
    if (s_RingBuffers_.containsObjectNamed(name)){
      warning("There is already a ring buffer named " + name + " open for writing. It will be replaced with this one.");
    }
#endif
    s_RingBuffers_.insertObject(name, ringBufferTable_);
  }
  
  void RingBufferWriter::reset()
  {
    ringBufferTable_.reset();
  }
  
} // Namespace Tonic
