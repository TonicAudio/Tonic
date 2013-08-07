//
//  ControlXYSpeed.cpp
//  Tonic
//
//  Created by Morgan Packard on 4/8/13.
//

#include "ControlXYSpeed.h"

namespace Tonic { namespace Tonic_{
  
  ControlXYSpeed_::ControlXYSpeed_(): lastYVal(0), lastXVal(0){
    
  }
  
  void  ControlXYSpeed_::setX(ControlGenerator xArg){
    x = xArg;
  }
  
  void  ControlXYSpeed_::setY(ControlGenerator yArg){
    y = yArg;
  }
  
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
