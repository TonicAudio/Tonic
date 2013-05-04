//
//  Mixer.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/9/13.

//

#include "Mixer.h"

namespace Tonic {
  
  Mixer::Mixer() : limitOutput_(false) {
    workSpace_.resize(kSynthesisBlockSize, 2, 0);
    limiter_.setIsStereo(true);
    pthread_mutex_init(&input_mutex_, NULL);
  }
  
  Mixer::~Mixer() {
    pthread_mutex_destroy(&input_mutex_);
  }
  
  void Mixer::addInput(BufferFiller* input)
  {
      // no checking for duplicates, maybe we should
    pthread_mutex_lock(&input_mutex_);
    inputs_.push_back(input);
    pthread_mutex_unlock(&input_mutex_);
  }
  
  void Mixer::removeInput(BufferFiller* input)
  {
    vector<BufferFiller*>::iterator it = std::find(inputs_.begin(), inputs_.end(), input);
    if (it != inputs_.end()){
      pthread_mutex_lock(&input_mutex_);
      inputs_.erase(it);
      pthread_mutex_unlock(&input_mutex_);
    }

  }
  
  void Mixer::tick( TonicFrames& frames, const Tonic_::SynthesisContext_ & context )
  {
    // Clear buffer
    memset(&frames[0], 0, frames.size() * sizeof(TonicFloat));
    
    // Tick and add inputs
    pthread_mutex_lock(&input_mutex_);
    for (unsigned int i=0; i<inputs_.size(); i++){
      // Tick each bufferFiller every time, with our context (for now).
      inputs_[i]->tick(workSpace_, context);
      frames += workSpace_;
    }
    pthread_mutex_unlock(&input_mutex_);

    
    if (limitOutput_){
      limiter_.tickThrough(frames);
    }
  }

  
} // Namespace Tonic
