//
//  RingBuffer.h
//  Tonic 
//
//  Created by Nick Donaldson on 5/20/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__RingBuffer__
#define __Tonic__RingBuffer__

#include "Effect.h"
#include "SampleTables.h"

namespace Tonic {
  
  namespace Tonic_ {
    
    class RingBufferTable_ : public SampleTable_ {
      
    private:
      
      unsigned long readHead_;
      unsigned long writeHead_;
      
    public:
      
      RingBufferTable_( unsigned int frames = 2, unsigned int channels = 1);
      void write(float * inFrames, unsigned int nSamples);
      void read(float * outFrames, unsigned int nSamples);
      void reset();
      
    };
    
    inline void RingBufferTable_::write(float *inFrames, unsigned int nSamples){
      
      TonicFloat *writePtr = &frames_[writeHead_];
      unsigned long dataSize = frames_.size();
      
      for (unsigned int i=0; i<nSamples; i++){
        *writePtr++ = *inFrames++;
        
#ifdef TONIC_DEBUG
        if (writeHead_ <= readHead_ && writeHead_+1 > readHead_){
          warning("RingBuffer overrun detected");
        }
#endif
        
        if (++writeHead_ >= dataSize){
          writeHead_ = 0;
          writePtr = &frames_[0];
        }
        
      }
      
    }
    
    inline void RingBufferTable_::read(float *outFrames, unsigned int nSamples){
      
      TonicFloat *readPtr = &frames_[readHead_];
      unsigned long dataSize = frames_.size();
      
      for (unsigned int i=0; i<nSamples; i++){
        *outFrames++ = *readPtr++;
        
#ifdef TONIC_DEBUG
        if (readHead_ <= writeHead_ && readHead_+1 > writeHead_){
          warning("RingBuffer underrun detected");
        }
#endif
        
        if (++readHead_ >= dataSize){
          readHead_ = 0;
          readPtr = &frames_[0];
        }
        
      }
    }
    
    inline void RingBufferTable_::reset(){
      readHead_ = 0;
      writeHead_ = 0;
    }
    
  }
  
  
  // ----------- Ring Buffer sample table -----------
  
  //! Like a SampleTable_, but with counters for over/underrun detection and easy synchronous read/write
  
  class RingBufferTable : public SampleTable {
    
  public:
    
    RingBufferTable(unsigned int nFrames = 2, unsigned int nChannels = 2){
      delete sampleTable_;
      sampleTable_ = new Tonic_::RingBufferTable_(nFrames, nChannels);
    }
    
    void write(float * inFrames, unsigned int nSamples){
      static_cast<Tonic_::RingBufferTable_*>(sampleTable_)->write(inFrames, nSamples);
    }
    
    void read(float * outFrames, unsigned int nSamples){
      static_cast<Tonic_::RingBufferTable_*>(sampleTable_)->read(outFrames, nSamples);
    }
    
    void reset(){
      static_cast<Tonic_::RingBufferTable_*>(sampleTable_)->reset();
    }
    
  };
  
// ----- End RingBufferTable implementations -----
  
  
  namespace Tonic_ {

    class RingBuffer_ : public Generator_{
      
    protected:
      
      RingBufferTable ringBufferTable_;
      
    public:
      
      RingBuffer_();
            
      void setRingBufferTable( RingBufferTable table) { ringBufferTable_ = table; }
      
      void write(float *inFrames, unsigned long nSamples) {
        ringBufferTable_.write(inFrames, nSamples);
      };
      
      void reset(){
        ringBufferTable_.reset();
      }
            
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    };
    
    inline void RingBuffer_::computeSynthesisBlock(const SynthesisContext_ &context){
      // get some output from the table
      ringBufferTable_.read(&outputFrames_[0], kSynthesisBlockSize);
    };
    
  }
  
  
  class RingBuffer : public TemplatedGenerator<Tonic_::RingBuffer_>{
    
  public:
        
    //! If name already exists in global sample table space, last two arguments are ignored.
    RingBuffer & initialize(string name, unsigned int nFrames = 0, unsigned int nChannels = 0);
        
    void write(float * inFrames, unsigned nSamples){
      gen()->write(inFrames, nSamples);
    }
    
    void reset(){
      gen()->reset();
    }
    
  };
  
}

#endif /* defined(__Tonic__RingBuffer__) */


