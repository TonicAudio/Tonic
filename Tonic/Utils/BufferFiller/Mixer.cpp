//
//  Mixer.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/9/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "Mixer.h"

namespace Tonic {
  
  Mixer::Mixer(){
    workSpace_.resize(kSynthesisBlockSize, 2, 0);
  }
  
  void Mixer::addInput(BufferFiller* input)
  {
      // no checking for duplicates, maybe we should
    inputs_.push_back(input);
  }
  
  void Mixer::removeInput(BufferFiller* input)
  {
    vector<BufferFiller*>::iterator it = std::find(inputs_.begin(), inputs_.end(), input);
    if (it != inputs_.end()){
      inputs_.erase(it);
    }

  }
  
  void Mixer::tick( TonicFrames& frames, const Tonic_::SynthesisContext_ & context )
  {
    // Clear buffer
    memset(&frames[0], 0, frames.size() * sizeof(TonicFloat));
    
    // Tick and add inputs
    for (unsigned int i=0; i<inputs_.size(); i++){
      // Tick each bufferFiller every time, with our context (for now).
      inputs_[i]->tick(workSpace_, context);
      frames += workSpace_;
    }
  }

  
} // Namespace Tonic
