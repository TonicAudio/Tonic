//
//  BufferFiller.h
//  Tonic
//
//  Created by Nick Donaldson on 2/9/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_BUFFERFILLER_H
#define TONIC_BUFFERFILLER_H

#include "Generator.h"

namespace Tonic{
  
  namespace Tonic_ {
    
    //! Base class for any generator expected to produce output for a buffer fill.
    /*!
     BufferFillers provide a high-level interface for combinations of generators, and can be used to fill
     arbitraryly large buffers.
     
     Subclasses to include mixer, channel, synth, etc.
     */
    class BufferFiller_ : public Generator_ {
      
    private:
      
      unsigned long               bufferReadPosition_;
      TONIC_MUTEX_T               mutex_;
      
    protected:
      
      Tonic_::SynthesisContext_   synthContext_;
      
    public:
      
      BufferFiller_();
      ~BufferFiller_();
      
      // mutex for swapping inputs, etc
      void lockMutex();
      void unlockMutex();
      
      //! Process a single synthesis vector, output to frames
      /*!
       tick method without context argument passes down this instance's SynthesisContext_
       */
      void tick( TonicFrames& frames );
      
      void fillBufferOfFloats(float *outData,  unsigned int numFrames, unsigned int numChannels);

    };
    
    inline void BufferFiller_::lockMutex(){
      TONIC_MUTEX_LOCK(mutex_);
    }
    
    inline void BufferFiller_::unlockMutex(){
      TONIC_MUTEX_UNLOCK(mutex_);
    }
    
    inline void BufferFiller_::tick( TonicFrames& frames ){
      lockMutex();
      Generator_::tick(frames, synthContext_);
      synthContext_.tick();
      unlockMutex();
    }
    
    inline void BufferFiller_::fillBufferOfFloats(float *outData,  unsigned int numFrames, unsigned int numChannels)
    {
      
      // flush denormals on this thread
      TONIC_ENABLE_DENORMAL_ROUNDING();
      
#ifdef TONIC_DEBUG
      if(numChannels > outputFrames_.channels()) error("Mismatch in channels sent to Synth::fillBufferOfFloats", true);
#endif
      
      const unsigned long sampleCount = outputFrames_.size();
      const unsigned int channelsPerSample = (outputFrames_.channels() - numChannels) + 1;
      
      TonicFloat sample = 0.0f;
      TonicFloat *outputSamples = &outputFrames_[bufferReadPosition_];
      
      for(unsigned int i = 0; i<numFrames * numChannels; i++){
        
        sample = 0;
        
        for (unsigned int c = 0; c<channelsPerSample; c++){
          if(bufferReadPosition_ == 0){
            tick(outputFrames_);
          }
          
          sample += *outputSamples++;
          
          if(++bufferReadPosition_ == sampleCount){
            bufferReadPosition_ = 0;
            outputSamples = &outputFrames_[0];
          }
        }
        
        *outData++ = sample / (float)channelsPerSample;
      }
    }
    
  }
  
  class BufferFiller : public Generator {
    
  public:
    
    BufferFiller(Tonic_::BufferFiller_ * newBf) : Generator(newBf) {}
        
    //! Fill an arbitrarily-sized, interleaved buffer of audio samples as floats
    /*!
     This BufferFiller's outputGen is used to fill an interleaved buffer starting at outData.
     */
    inline void fillBufferOfFloats(float *outData,  unsigned int numFrames, unsigned int numChannels){
      static_cast<Tonic_::BufferFiller_*>(obj)->fillBufferOfFloats(outData, numFrames, numChannels);
    }
  
  };
  
  template<class GenType>
  class TemplatedBufferFiller : public BufferFiller {
  protected:
    GenType* gen(){
      return static_cast<GenType*>(obj);
    }
  public:
    TemplatedBufferFiller() : BufferFiller(new GenType) {}
  };
  
}

#endif /* defined(__TonicDemo__BufferFiller__) */
