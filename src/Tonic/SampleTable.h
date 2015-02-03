//
//  SampleTable.h
//  Tonic
//
//  Created by Nick Donaldson on 5/17/13.
//
//
// See LICENSE.txt for license and usage information.
//

// Global storage for sampled audio data - wavetables, in-memory samples, ring buffers, audio input, etc
// Allows registration of a TonicFrames object with a name (string)

#ifndef TONIC_SAMPLETABLE_H
#define TONIC_SAMPLETABLE_H

#include "TonicFrames.h"

namespace Tonic {
  
// -------
  
  namespace Tonic_ {
    
    class SampleTable_ {
      
    protected:
      TonicFrames frames_;
      
    public:
      
      SampleTable_(unsigned int frames, unsigned int channels);
      
      // Property getters
      unsigned int channels() const {
        return frames_.channels();
      }
      
      unsigned long frames() const {
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
      
      // Resample
      void resample(unsigned int frames, unsigned int channels){
        frames_.resample(frames, channels);
      }
      
    };
    
  }
  
  //! Access to a persistent TonicFrames instance, so the same audio data can be read from multiple places
  class SampleTable : public TonicSmartPointer<Tonic_::SampleTable_>
  {
    
  public:
    
    SampleTable(unsigned int nFrames = 64, unsigned int nChannels = 2) : TonicSmartPointer<Tonic_::SampleTable_>( new Tonic_::SampleTable_(nFrames, nChannels) ) {}
  
    // Property getters
    unsigned int channels() const {
      return obj->channels();
    }
  
    unsigned long frames() const {
      return obj->frames();
    }
  
    size_t size() const {
      return obj->size();
    }
  
    // Pointer to start of data array
    TonicFloat * dataPointer() {
      return obj->dataPointer();
    }
  
    // Resize
    void resize(unsigned int frames, unsigned int channels){
      obj->resize(frames, channels);
    }
    
    // Resample
    void resample(unsigned int frames, unsigned int channels){
      obj->resample(frames, channels);
    }
    
  };

}


#endif /* defined(__TonicLib__SampleTables__) */
