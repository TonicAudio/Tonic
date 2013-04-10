//
//  DelayUtils.cpp
//  Tonic
//
//  Created by Nick Donaldson on 3/10/13.

//

#include "DelayUtils.h"

namespace Tonic {
  
  DelayLine::DelayLine() :
    lastDelayTime_(0),
    readHead_(0),
    writeHead_(0),
    isInitialized_(false),
    interpolates_(true)
  {

  }
  
  void DelayLine::initialize(float initialDelay, float maxDelay, unsigned int channels)
  {
    unsigned int nFrames = max(2, maxDelay * Tonic::sampleRate());
    resize(nFrames, channels, 0);
    isInitialized_ = true;
    advance(initialDelay);
  }
  
  void DelayLine::clear()
  {
    if (isInitialized_){
      memset(data_, 0, size_ * sizeof(TonicFloat));
    }
  }
  
}