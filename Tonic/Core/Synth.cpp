//
//  Synth.cpp
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/23/13.
//
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

#include "Synth.h"
#include "SineWave.h"

namespace Tonic {

  Synth::Synth() : synthBufferReadPosition(0){
    outputFrames.resize(kSynthesisBlockSize, 2);
  }

  void Synth::tick( TonicFrames& frames ){
        outputGen.tick(frames);
  };

      // fill a buffer of floats, assuming the buffer is expecting max/min of 1,-1
  void Synth::fillBufferOfFloats(float *outData,  UInt32 numFrames, int numChannels){
    
    
    if(numChannels > outputFrames.channels()) error("Mismatch in channels sent to Synth::fillBufferOfFloats");
    
    const unsigned int sampleCount = outputFrames.channels() * outputFrames.frames();
    const unsigned int channelsPerSample = (outputFrames.channels() - numChannels) + 1;
      
    TonicFloat sample = 0.0f;
    
    for(unsigned int i = 0; i<numFrames * numChannels; i++){
        
        sample = 0;
        
        for (unsigned int c = 0; c<channelsPerSample; c++){
            if(synthBufferReadPosition == 0){
                tick(outputFrames);
            }
            
            sample += outputFrames[synthBufferReadPosition++];
            
            if(synthBufferReadPosition == sampleCount){
                synthBufferReadPosition = 0;
            }
        }
        
        *outData++ = sample / (float)channelsPerSample;
    }
    
  }

  void  Synth::setOutputGen(Generator gen){
    outputGen = gen;
  }


  ControlValue  Synth::registerMessage(string name, float value){
    if (parameters.find(name)==parameters.end()) {
        parameters[name] = ControlValue().setValue(value);
    }
    return parameters[name];
  }
  
  void Synth::sendMessage(string name, float value){
    if (parameters.find(name)!=parameters.end()) {
      parameters[name].setValue(value);
    }else{
      error("message: " + name + " was not registered. You can register a message using Synth::registerMessage.");
    }
  }
  
}
