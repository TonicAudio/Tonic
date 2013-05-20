//
//  Mixer.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/9/13.

//

#include "Mixer.h"

namespace Tonic {
  
  namespace Tonic_ { 
  
    Mixer_::Mixer_() {
      workSpace_.resize(kSynthesisBlockSize, 2, 0);
    }
    
    void Mixer_::addInput(BufferFiller input)
    {
      // no checking for duplicates, maybe we should
      inputs_.push_back(input);
    }
    
    void Mixer_::removeInput(BufferFiller input)
    {
      vector<BufferFiller>::iterator it = std::find(inputs_.begin(), inputs_.end(), input);
      if (it != inputs_.end()){
        inputs_.erase(it);
      }
    }
  }
  
} // Namespace Tonic
