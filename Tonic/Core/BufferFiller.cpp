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
  
  BufferFiller::BufferFiller() : bufferReadPosition(0){
    outputFrames.resize(kSynthesisBlockSize, 2);
    outputGen = PassThroughGenerator();
  }
  
  
  inline void BufferFiller::tick( TonicFrames& frames ){
    outputGen.tick(frames);
  };
  
  // fill a buffer of floats, assuming the buffer is expecting max/min of 1,-1
  void BufferFiller::fillBufferOfFloats(float *outData,  UInt32 numFrames, int numChannels){
  
    if(numChannels > outputFrames.channels()) error("Mismatch in channels sent to Synth::fillBufferOfFloats");
    
    const unsigned int sampleCount = outputFrames.channels() * outputFrames.frames();
    const unsigned int channelsPerSample = (outputFrames.channels() - numChannels) + 1;
    
    TonicFloat sample = 0.0f;
    TonicFloat *outputSamples = &outputFrames[bufferReadPosition];
    
    for(unsigned int i = 0; i<numFrames * numChannels; i++){
      
      sample = 0;
      
      for (unsigned int c = 0; c<channelsPerSample; c++){
        if(bufferReadPosition == 0){
          tick(outputFrames);
        }
        
        sample += *outputSamples++;
        
        if(++bufferReadPosition == sampleCount){
          bufferReadPosition = 0;
          outputSamples = &outputFrames[0];
        }
      }
      
      *outData++ = sample / (float)channelsPerSample;
    }
    
  }
  
}