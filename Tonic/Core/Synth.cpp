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


template<typename T>T&  Synth::a(T* gen){
 return *gen;
}

void Synth::tick( TonicFrames& frames ){

// TODO -- add sequencers back in
//        for (int i = 0; i < sequencers.size(); i++) {
//          sequencers.at(i)->advanceBySamples(frames.frames());
//        }
      outputGen.tick(frames);
  
// TODO -- add message queue back in
//        while (!messageQueue.empty()) {
//            EZPlugSynthMessage msg = messageQueue.pop_front();
//            switch (msg.messageType) {
//                case SET_FIXED_VALUE:{
//                    std::string target = msg.target;
//                    pSetFixedValue(target, msg.value);
//                    break;
//                }
//                case TRIGGER:{
//                    std::string target = msg.target;
//                    pTrigger(target);
//                    break;
//                }
//            }
//        }
  
  

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


  
}
