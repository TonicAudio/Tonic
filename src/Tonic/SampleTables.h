//
//  SampleTables.h
//  TonicLib
//
//  Created by Nick Donaldson on 5/17/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

// Global storage for sampled audio data - wavetables, in-memory samples, ring buffers, audio input, etc
// Allows registration of a TonicFrames object with a name (string)

#ifndef __TonicLib__SampleTables__
#define __TonicLib__SampleTables__

#include "TonicFrames.h"

namespace Tonic {
  
  // forward declaration
  class SampleTable;
  
  namespace Tonic_ {
    
    // NOTE: Should not have to worry about Mutexes for simultaneous R/W (yet) since audio input/output threads are typically synchronous
    // and UI/other threads should not be modifying sample tables directly.
    
    // Returns reference to new sample table, or pointer to existing sample table if one is already created under the same name
    // AUTOMATICALLY INCREASES RETAIN COUNT ON NEW TABLE
    extern SampleTable createSampleTable(std::string name, unsigned int length = 2048, unsigned int channels = 1);
    
    // Assings refernce argument to existing sample table. Returns false and does not modify the referenceif name has not been registered.
    // DOES NOT MODIFY RETAIN COUNT FOR TABLE
    extern bool getSampleTable(std::string name, SampleTable * table);
    
    // retain count management
    extern void retainSampleTable(std::string name);
    extern void releaseSampleTable(std::string name);
   
  }

  // smart pointer for sample tables
  class SampleTable{
    
  protected:
    
    TonicFrames * frames_;
    int* pcount;
    
  public:
    SampleTable(unsigned int length = 2, unsigned int channels = 1) : frames_(new TonicFrames(0, length, channels)), pcount(new int(1)) {}
    SampleTable(const SampleTable& r): frames_(r.frames_), pcount(r.pcount){(*pcount)++;}
    SampleTable& operator=(const SampleTable& r)
    {
      if(frames_ == r.frames_) return *this;
      
      if(--(*pcount) == 0){
        delete frames_;
        delete pcount;
      }
      
      frames_ = r.frames_;
      pcount = r.pcount;
      (*pcount)++;
      return *this;
    }
    
    ~SampleTable(){
      if(--(*pcount) == 0){
        delete frames_;
        delete pcount;
      }
    }
    
    bool operator==(const SampleTable& r){
      return frames_ == r.frames_;
    }
  
    // Property getters
    unsigned int channels() const {
      return frames_->channels();
    }
  
    unsigned int frames() const {
      return frames_->frames();
    }
  
    size_t size() const {
      return frames_->size();
    }
  
    // Pointer to start of data array
    TonicFloat * dataPointer() {
      return &(*frames_)[0];
    }
  
    // Resize
    void resize(unsigned int frames, unsigned int channels){
      frames_->resize(frames, channels);
    }
    
  };

}


#endif /* defined(__TonicLib__SampleTables__) */
