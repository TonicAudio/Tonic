//
//  ControlMidiToFreq.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 3/5/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "ControlMidiToFreq.h"


namespace Tonic{

  namespace Tonic_{
  
    void ControlMidiToFreq_::computeOutput(const SynthesisContext_ & context){
      ControlGeneratorOutput inputOut = input_.tick(context);
      lastOutput_.status = inputOut.status;
      lastOutput_.value = mtof( inputOut.value );
    }
    
  }
}
