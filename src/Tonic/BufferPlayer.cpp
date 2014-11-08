//
//  BufferPlayer.cpp
//  Tonic
//
//  Created by Morgan Packard on 10/26/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "BufferPlayer.h"
#include "ControlTrigger.h"

namespace Tonic { namespace Tonic_{
  
  BufferPlayer_::BufferPlayer_() : currentSample(0), isFinished_(true){
    doesLoop_ = ControlValue(false);
    trigger_ = ControlTrigger();
    startPosition_ = ControlValue(0);
  }
  
  BufferPlayer_::~BufferPlayer_(){
    
  }
  
  void  BufferPlayer_::setBuffer(SampleTable buffer){
    buffer_ = buffer;
    setIsStereoOutput(buffer.channels() == 2);
    samplesPerSynthesisBlock = kSynthesisBlockSize * buffer_.channels();
  }
  
  inline void BufferPlayer_::computeSynthesisBlock(const SynthesisContext_ &context){
    
    bool doesLoop = doesLoop_.tick(context).value;
    bool trigger = trigger_.tick(context).triggered;
    float startPosition = startPosition_.tick(context).value;
    
    if(trigger){
      isFinished_ = false;
      currentSample = startPosition * sampleRate() * buffer_.channels();
    }
    
    if(isFinished_){
      outputFrames_.clear();
    }else{
      int samplesLeftInBuf = (int)buffer_.size() - currentSample;
      int samplesToCopy = min(samplesPerSynthesisBlock, samplesLeftInBuf);
      copySamplesToOutputBuffer(currentSample, samplesToCopy);
      if (samplesToCopy < samplesPerSynthesisBlock) {
        if(doesLoop){
          currentSample = 0;
        }else{
          isFinished_ = true;
        }
      }else{
        currentSample += samplesPerSynthesisBlock;
      }
    }
  }


} // Namespace Tonic_
  
  
  
} // Namespace Tonic
