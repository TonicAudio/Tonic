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
  }
  
  void Reverb_::setInput(Generator input){
    Effect_::setInput(input);
    setIsStereoInput(input.isStereoOutput());
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
