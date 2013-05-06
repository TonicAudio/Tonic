//
//  Reverb.cpp
//  Tonic
//
//  Created by Nick Donaldson on 5/4/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "Reverb.h"

namespace Tonic { namespace Tonic_{
  
  Reverb_::Reverb_(){
    setIsStereoOutput(true);
    
    preDelayLine_.initialize(0.f, 0.25f);
    reflectDelayLine_.initialize(0.f, 0.25f);
    
    workSpace_[0].resize(kSynthesisBlockSize, 1, 0);
    workSpace_[1].resize(kSynthesisBlockSize, 1, 0);
  }
  
  void Reverb_::setInput(Generator input){
    Effect_::setInput(input);
    setIsStereoInput(input.isStereoOutput());
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
