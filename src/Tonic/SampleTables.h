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
#include <map>

namespace Tonic {
  
  // forward declarations
  class SampleTableCollection;
  class SampleTable;
  
  //! Shared sample table collection, globally accessible.
  extern SampleTableCollection & SharedSampleTables();
  
  //! A collection of named sample tables.
  /*!
   Individual instances can be used in more localized scopes to maintain a collection of sample data.
   Use SharedSampleTables() to get the globally-shared collection.
   */
  class SampleTableCollection {
    
  protected:
    
    typedef std::map<std::string, SampleTable> SampleTableMap;
    SampleTableMap sampleTables_;
    
  public:
    
    //! Register a sample table. Does nothing if one is already created under the same name.
    void registerSampleTable(std::string name, SampleTable table);
    
    //! Assigns pointer to existing sample table. Returns false and does not modify the pointer if name has not been registered.
    bool getSampleTable(std::string name, SampleTable * table);
    
    //! Unregisters sample table from collection.
    void unregisterSampleTable(std::string name);
    
  };
  

// -------
  
  
  namespace Tonic_ {
    
    class SampleTable_ {
      
    protected:
      TonicFrames frames_;
      
    public:
      
      SampleTable_(unsigned int frames = 2, unsigned int channels = 1);
      
      // Property getters
      unsigned int channels() const {
        return frames_.channels();
      }
      
      unsigned int frames() const {
        return frames_.frames();
      }
      
      size_t size() const {
        return frames_.size();
      }
      
      // Pointer to start of data array
      TonicFloat * dataPointer() {
        return &frames_[0];
      }
      
      // Resize
      void resize(unsigned int frames, unsigned int channels){
        frames_.resize(frames, channels);
      }
      
    };
    
  }
  
  //! Sample Table object. Acts as a smart pointer for a persistent TonicFrames instance.
  class SampleTable {
    
  protected:
    
    Tonic_::SampleTable_ * sampleTable_;
    int* pcount;
    
  public:
    
    SampleTable(unsigned int length = 2, unsigned int channels = 1) : sampleTable_(new Tonic_::SampleTable_(length, channels)), pcount(new int(1)) {}
    SampleTable(const SampleTable& r): sampleTable_(r.sampleTable_), pcount(r.pcount){(*pcount)++;}
    SampleTable& operator=(const SampleTable& r)
    {
      if(sampleTable_ == r.sampleTable_) return *this;
      
      if(--(*pcount) == 0){
        delete sampleTable_;
        delete pcount;
      }
      
      sampleTable_ = r.sampleTable_;
      pcount = r.pcount;
      (*pcount)++;
      return *this;
    }
    
    ~SampleTable(){
      if(--(*pcount) == 0){
        delete sampleTable_;
        delete pcount;
      }
    }
    
    bool operator==(const SampleTable& r){
      return sampleTable_ == r.sampleTable_;
    }
  
    // Property getters
    unsigned int channels() const {
      return sampleTable_->channels();
    }
  
    unsigned int frames() const {
      return sampleTable_->frames();
    }
  
    size_t size() const {
      return sampleTable_->size();
    }
  
    // Pointer to start of data array
    TonicFloat * dataPointer() {
      return sampleTable_->dataPointer();
    }
  
    // Resize
    void resize(unsigned int frames, unsigned int channels){
      sampleTable_->resize(frames, channels);
    }
    
  };

}


#endif /* defined(__TonicLib__SampleTables__) */
