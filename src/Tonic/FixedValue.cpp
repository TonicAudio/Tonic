//
//  FixedValue.cpp
//  Tonic
//
//  Created by Morgan Packard on 1/27/13.

//
// See LICENSE.txt for license and usage information.
//


#include "FixedValue.h"

namespace Tonic {
  
  namespace Tonic_ {
    
    FixedValue_::FixedValue_(float val){
      valueGen = ControlValue(val);
    }
    
    
  }
}