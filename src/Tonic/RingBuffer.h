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
#include "SampleTable.h"

namespace Tonic {
  
  namespace Tonic_ {
    
    class RingBufferTable_ : public SampleTable_ {
      
    private:
      
      unsigned long readHead_;
      unsigned long writeHead_;
      
    public:
      
      RingBufferTable_( unsigned int frames, unsigned int channels );
      void write(float * data, unsigned int nFrames, unsigned int nChannels);
      void read(TonicFrames & outFrames);
      void reset();
      
    };
    
    inline void RingBufferTable_::write(TonicFloat *data, unsigned int nFrames, unsigned int nChannels){
      
#ifdef TONIC_DEBUG
      // Detect overrun
      unsigned int finalWriteHead = (writeHead_ + nFrames) % frames();
      if (finalWriteHead >= readHead_ && (writeHead_ < readHead_ || finalWriteHead < writeHead_)){
        warning("RingBuffer overrun detected");
      }
#endif
      
      TonicFloat *writeptr = &frames_(writeHead_, 0);
      
      unsigned int bufChannels = channels();
      unsigned int bufFrames = frames();
      
      if (bufChannels == nChannels){
        
        for (unsigned int i=0; i<nFrames; i++){
          for (unsigned int c=0; c<nChannels; c++){
            *writeptr++ = *data++;
          }
          if (++writeHead_ >= bufFrames){
            writeHead_ = 0;
            writeptr = &frames_[0];
          }
        }
        
      }
      else if (bufChannels > nChannels){
        for (unsigned int i=0; i<nFrames; i++){
          for (unsigned int c=0; c<bufChannels; c++){
            *writeptr++ = *data;
          }
          data++;
          if (++writeHead_ >= bufFrames){
            writeHead_ = 0;
            writeptr = &frames_[0];
          }
        }
      }
      else {
        for (unsigned int i=0; i<nFrames; i++){
          *writeptr = 0;
          for (unsigned int c=0; c<nChannels; c++){
            *writeptr += *data++;
          }
          *writeptr++ /= (float)nChannels;
          if (++writeHead_ >= bufFrames){
            writeHead_ = 0;
            writeptr = &frames_[0];
          }
        }
      }
      
    }
    
    inline void RingBufferTable_::read(TonicFrames & outFrames){
      
#ifdef TONIC_DEBUG
      // Detect underrun
      unsigned int finalReadHead = (readHead_ + outFrames.frames()) % frames();
      if (finalReadHead > writeHead_ && (readHead_ < writeHead_ || finalReadHead < readHead_)){
        warning("RingBuffer underrun detected");
      }
#endif
      
      TonicFloat *readptr = &frames_(readHead_, 0);
      TonicFloat *outptr = &outFrames[0];
      
      unsigned int nFrames = outFrames.frames();
      unsigned int nChannels = outFrames.channels();
      
      unsigned int bufChannels = channels();
      unsigned int bufFrames = frames();
      
      if (bufChannels == nChannels){
        
        for (unsigned int i=0; i<nFrames; i++){
          for (unsigned int c=0; c<nChannels; c++){
            *outptr++ = *readptr++;
          }
          if (++readHead_ >= bufFrames){
            readHead_ = 0;
            readptr = &frames_[0];
          }
        }
        
      }
      else if (bufChannels > nChannels){
        for (unsigned int i=0; i<nFrames; i++){
          *outptr = 0;
          for (unsigned int c=0; c<bufChannels; c++){
            *outptr += *readptr++;
          }
          *outptr /= (float)bufChannels;
          outptr++;
          if (++readHead_ >= bufFrames){
            readHead_ = 0;
            readptr = &frames_[0];
          }
        }
      }
      else {
        for (unsigned int i=0; i<nFrames; i++){
          for (unsigned int c=0; c<nChannels; c++){
            *outptr++ = *readptr;
          }
          readptr++;
          if (++readHead_ >= bufFrames){
            readHead_ = 0;
            readptr = &frames_[0];
          }
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
  //  TODO: Maybe template the SampleTable smart pointer instead of statically casting the object.
  class RingBufferTable : public SampleTable {
    
  public:
    
    RingBufferTable(unsigned int nFrames = 2, unsigned int nChannels = 2){
      obj = new Tonic_::RingBufferTable_(nFrames, nChannels);
    }
    
    void write(TonicFloat * data, unsigned int nFrames, unsigned int nChannels){
      static_cast<Tonic_::RingBufferTable_*>(obj)->write(data, nFrames, nChannels);
    }
    
    void read(TonicFrames & outFrames){
      static_cast<Tonic_::RingBufferTable_*>(obj)->read(outFrames);
    }
    
    void reset(){
      static_cast<Tonic_::RingBufferTable_*>(obj)->reset();
    }
    
  };
  
// ----- End RingBufferTable implementations -----
  
  namespace Tonic_ {

    class RingBufferReader_ : public Generator_{
      
    protected:
      
      RingBufferTable ringBufferTable_;
      
    public:
                  
      void setRingBufferTable( RingBufferTable table) { ringBufferTable_ = table; }
      
      void reset(){
        ringBufferTable_.reset();
      }
            
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    };
    
    inline void RingBufferReader_::computeSynthesisBlock(const SynthesisContext_ &context){
      // get some output from the table
      ringBufferTable_.read(outputFrames_);
    };
    
  }
  
  
  class RingBufferReader : public TemplatedGenerator<Tonic_::RingBufferReader_>{
    
    public:
        
      RingBufferReader & bufferName(string name);
    
  };
  
  class RingBufferWriter {
      
    protected:
      
      RingBufferTable ringBufferTable_;
      
    public:
    
      RingBufferWriter();
      RingBufferWriter(string name, unsigned int nFrames, unsigned int nChannels);
      
      void write(float *data, unsigned int nFrames, unsigned int nChannels);
      void reset();
  };
  
  inline void RingBufferWriter::write(float *data, unsigned int nFrames, unsigned int nChannels)
  {
    ringBufferTable_.write(data, nFrames, nChannels);
  }
    
}

#endif /* defined(__Tonic__RingBuffer__) */


