//
//  BufferFiller.cpp
//  TonicDemo
//
//  Created by Nick Donaldson on 2/9/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

/*+++++++++++++++++++++ License ++++++++++++++++++++
 
 Use this code for whatever you want. There are NO
 RESTRICTIONS WHATSOVER. Modify it, repackage it,
 sell it, get rich from it, whatever. Go crazy. If
 you want to make mehappy, contribute to this
 project, but feel free to just use the code as a
 starting point for whatever you like.
 
 Note that Tonic is heavily indebted to STK
 https://ccrma.stanford.edu/software/stk/
 
 ++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "BufferFiller.h"

namespace Tonic {
  
  BufferFiller::BufferFiller() : bufferReadPosition_(0){
    outputFrames_.resize(kSynthesisBlockSize, 2);
    outputGen = PassThroughGenerator();
  }
  
  inline void BufferFiller::tick( TonicFrames& frames ){
    outputGen.tick(frames, context_);
    context_.tick();
  }
  
  inline void BufferFiller::tick( TonicFrames& frames, const Tonic_::SynthesisContext_ & context ){
    outputGen.tick(frames, context);
  }

  // fill a buffer of floats, assuming the buffer is expecting max/min of 1,-1
  void BufferFiller::fillBufferOfFloats(float *outData,  UInt32 numFrames, int numChannels){
  
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