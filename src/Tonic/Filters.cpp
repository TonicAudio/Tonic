//
//  Filters.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/22/13.

//

#include "Filters.h"

namespace Tonic { namespace Tonic_{
  
  // ================================
  //       Filter Base Class
  // ================================
  
  Filter_::Filter_() :
    cutoff_(FixedValue(20000)),
    Q_(FixedValue(0.7071)),
    bypass_(ControlValue(0)),
    bNormalizeGain_(true)
  {
    workspace_.resize(kSynthesisBlockSize, 1, 0);
  }
  
  void Filter_::setInput(Generator input){
    Effect_::setInput(input);
    setIsStereoInput(input.isStereoOutput());
    setIsStereoOutput(input.isStereoOutput());
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
