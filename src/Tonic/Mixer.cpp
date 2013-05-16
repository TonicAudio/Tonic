//
//  Mixer.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/9/13.

//

#include "Mixer.h"

namespace Tonic {
  
  Mixer::Mixer() {
    workSpace_.resize(kSynthesisBlockSize, 2, 0);
    TONIC_MUTEX_INIT(&input_mutex_);
  }
  
  Mixer::~Mixer() {
    TONIC_MUTEX_DESTROY(&input_mutex_);
  }
  
  void Mixer::addInput(BufferFiller* input)
  {
      // no checking for duplicates, maybe we should
    TONIC_MUTEX_LOCK(&input_mutex_);
    inputs_.push_back(input);
    TONIC_MUTEX_UNLOCK(&input_mutex_);
  }
  
  void Mixer::removeInput(BufferFiller* input)
  {
    vector<BufferFiller*>::iterator it = std::find(inputs_.begin(), inputs_.end(), input);
    if (it != inputs_.end()){
      TONIC_MUTEX_LOCK(&input_mutex_);
      inputs_.erase(it);
      TONIC_MUTEX_UNLOCK(&input_mutex_);
    }

  }
  
  void Mixer::tick( TonicFrames& frames, const Tonic_::SynthesisContext_ & context )
  {
    // Clear buffer
    memset(&frames[0], 0, frames.size() * sizeof(TonicFloat));
    
    // Tick and add inputs
    TONIC_MUTEX_LOCK(&input_mutex_);
    for (unsigned int i=0; i<inputs_.size(); i++){
      // Tick each bufferFiller every time, with our context (for now).
      inputs_[i]->tick(workSpace_, context);
      frames += workSpace_;
    }
    TONIC_MUTEX_UNLOCK(&input_mutex_);
  }

  
} // Namespace Tonic
