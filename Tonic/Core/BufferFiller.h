//
//  BufferFiller.h
//  Tonic
//
//  Created by Nick Donaldson on 2/9/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__BufferFiller__
#define __Tonic__BufferFiller__

#include <map>
#include "Generator.h"

//! BufferFiller is the base class for any generator expected to produce output for a buffer fill.
/*!
 BufferFillers provide a high-level interface for combinations of generators, and can be used to fill
 arbitraryly large buffers.
  
 Subclasses to include mixer, channel, synth, etc.
*/
namespace Tonic{
  
  class BufferFiller : public Generator {
    
  private:
    int                         bufferReadPosition_;
    TonicFrames                 outputFrames_;
    Tonic_::SynthesisContext_   context_;
    
  protected:
    
    Generator     outputGen;
    
  public:
    BufferFiller();
        
    //! tick method without context argument passes down this instance's SynthesisContext_
    void tick( TonicFrames& frames );
    
    //! tick method with context argument uses passed-in context
    virtual void tick( TonicFrames& frames, const Tonic_::SynthesisContext_ & context );
    
    //! Fill an arbitrarily-sized, interleaved buffer of audio samples as floats
    /*!
        This BufferFiller's outputGen is used to fill an interleaved buffer starting at outData.
    */
    void fillBufferOfFloats(float *outData,  unsigned int numFrames, unsigned int numChannels);
    
    //! Returns a reference to outputGen
    const Generator & getOutputGenerator() { return outputGen; };
  };
  
  inline void BufferFiller::tick( TonicFrames& frames ){
    this->tick(frames, context_);
    context_.tick();
  }
  
  inline void BufferFiller::tick( TonicFrames& frames, const Tonic_::SynthesisContext_ & context ){
    outputGen.tick(frames, context);
  }
  
  // fill a buffer of floats, assuming the buffer is expecting max/min of 1,-1
  inline void BufferFiller::fillBufferOfFloats(float *outData,  unsigned int numFrames, unsigned int numChannels){
    
    if(numChannels > outputFrames_.channels()) error("Mismatch in channels sent to Synth::fillBufferOfFloats", true);
    
    const unsigned int sampleCount = outputFrames_.channels() * outputFrames_.frames();
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

#endif /* defined(__TonicDemo__BufferFiller__) */
