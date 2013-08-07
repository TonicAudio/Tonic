//
//  RingBuffer.cpp
//  Tonic
//
//  Created by Nick Donaldson on 5/20/13.
//

#include "RingBuffer.h"

namespace Tonic {
  
  //! Registry of ring buffers
  TonicDictionary<RingBuffer> s_RingBuffers_;
  
  namespace Tonic_ {
    
    RingBuffer_::RingBuffer_(unsigned int frames, unsigned int channels) :
      SampleTable_(frames, channels),
      writeHead_(0),
      readHead_(0)
    {}
    
    RingBufferWriter_::~RingBufferWriter_(){
      s_RingBuffers_.removeObjectNamed(tableName_);
    }
    
    void RingBufferWriter_::initRingBuffer(string name, unsigned int nFrames, unsigned int nChannels){
     
      RingBuffer table = RingBuffer(nFrames, nChannels);
      
      // overwrite existing entry if there is one
#ifdef TONIC_DEBUG
      if (s_RingBuffers_.containsObjectNamed(name)){
        warning("There is already a ring buffer named " + name + " open for writing. It will be replaced with this one.");
      }
#endif
      s_RingBuffers_.insertObject(name, table);
      
      ringBuffer_ = table;
    }
    
    void RingBufferWriter_::reset()
    {
      ringBuffer_.reset();
    }
  }
  
  RingBufferReader & RingBufferReader::bufferName(string name){

    if (s_RingBuffers_.containsObjectNamed(name)){      
      gen()->setRingBuffer(s_RingBuffers_.objectNamed(name));
    }
    else{
      error("RingBuffer named " + name + " does not exist.");
    }
    return *this;
  }
  
  RingBufferWriter::RingBufferWriter(string name, unsigned int nFrames, unsigned int nChannels)
  {
    obj = new Tonic_::RingBufferWriter_();
    obj->initRingBuffer(name, nFrames, nChannels);
  }
  
} // Namespace Tonic
