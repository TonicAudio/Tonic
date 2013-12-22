//
//  ControlRecorder.cpp
//  Tonic
//
//  Created by Morgan Packard on 4/11/13.
//
//

#include "ControlRecorder.h"

namespace Tonic { namespace Tonic_{
  
  ControlRecorder_::ControlRecorder_() : playbackHead(0) {}
  
  void  ControlRecorder_::setMode(ControlGenerator modeArg){
    mode = modeArg;
  }
  
} // Namespace Tonic_
  
} // Namespace Tonic
