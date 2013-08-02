
//
//  Effect.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/20/13.

//
// See LICENSE.txt for license and usage information.
//

#include "Effect.h"

namespace Tonic
{
  
  namespace Tonic_
  {
   
    Effect_::Effect_() : isStereoInput_(false)
    {
      dryFrames_.resize(kSynthesisBlockSize, 1, 0);
      bypassGen_ = ControlValue(0);
    }
    
    WetDryEffect_::WetDryEffect_()
    {
      mixWorkspace_.resize(kSynthesisBlockSize, 1, 0);
      dryLevelGen_ = FixedValue(0.5);
      wetLevelGen_ = FixedValue(0.5);
    }
    
  }
  
}