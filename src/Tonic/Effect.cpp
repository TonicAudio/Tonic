
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
   
    Effect_::Effect_() : isAlwaysWet_(false), isStereoInput_(false){
      dryFrames_.resize(kSynthesisBlockSize, 1, 0);
      mixWorkspace_.resize(kSynthesisBlockSize, 1, 0);
      bypassGen_ = ControlValue(0);
      dryLevelGen_ = FixedValue(0.0);
      wetLevelGen_ = FixedValue(1.0);
    }
    
  }
}