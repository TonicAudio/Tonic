
//
//  Effect.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/20/13.

//
// See LICENSE.txt for license and usage information.
//

#include "Effect.h"

namespace Tonic {
  namespace Tonic_ {
   
    Effect_::Effect_(){
      dryFrames_.resize(kSynthesisBlockSize, 1, 0);
      bypassGen_ = ControlValue(0);
    }
    
  }
}