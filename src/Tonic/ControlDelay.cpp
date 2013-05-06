//
//  ControlDelay.cpp
//  Tonic
//
//  Created by Nick Donaldson on 5/5/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "ControlDelay.h"

namespace Tonic { namespace Tonic_{
  
  ControlDelay_::ControlDelay_() :
    readHead_(0),
    writeHead_(0),
    maxDelay_(0)
  {}
  
  void ControlDelay_::initialize(float maxDelayTime){
    maxDelay_ = max(maxDelayTime * sampleRate() / kSynthesisBlockSize, 1);
    delayLine_.resize(maxDelay_);
  }
  
  void ControlDelay_::computeOutput(const SynthesisContext_ & context){
    
    delayLine_[writeHead_] = input_.tick(context);
    
    ControlGeneratorOutput delayTimeOutput = delayTimeCtrlGen_.tick(context);
    if (delayTimeOutput.status == ControlGeneratorStatusHasChanged){
      
      unsigned delayBlocks = max(delayTimeOutput.value * sampleRate() / kSynthesisBlockSize, 1);
      
#ifdef TONIC_DEBUG
      if (delayBlocks >= maxDelay_){
        debug("ControlDelay: delay time greater than maximum delay (defaults to 1 scond). Use constructor to set max delay -- ex. ControlDelay(2.0))"); 
      }
#endif
      
      readHead_ = writeHead_ - delayBlocks;
      if (readHead_ < 0) readHead_ += maxDelay_;
      
    }
    
    lastOutput_ = delayLine_[readHead_];
    
    if (++writeHead_ >= maxDelay_) writeHead_ = 0;
    if (++readHead_ >= maxDelay_) readHead_ = 0;
  }
  
} // Namespace Tonic_
  
  ControlDelay::ControlDelay(float maxDelayTime){
    this->gen()->initialize(maxDelayTime);
  }
  
} // Namespace Tonic
