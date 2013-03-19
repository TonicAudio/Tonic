//
//  DelayUtils.cpp
//  Tonic
//
//  Created by Nick Donaldson on 3/10/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "DelayUtils.h"

namespace Tonic {
  
  DelayLine::DelayLine(float initialDelay, float maxDelay, unsigned int channels) :
    lastDelayTime_(0),
    readHead_(0),
    writeHead_(0)
  {
    unsigned int nFrames = max(2, maxDelay * Tonic::sampleRate());
    resize(nFrames, channels, 0);
    advance(initialDelay);
  }
  
  void DelayLine::clear()
  {
    memset(data_, 0, size_ * sizeof(TonicFloat));
  }
  
}