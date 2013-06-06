//
//  ControlMidiToFreq.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 3/5/13.

//

#include "ControlMidiToFreq.h"


namespace Tonic{

  namespace Tonic_{
  
    void ControlMidiToFreq_::computeOutput(const SynthesisContext_ & context){
      ControlGeneratorOutput inputOut = input_.tick(context);
      output_.triggered = inputOut.triggered;
      output_.value = mtof( inputOut.value );
    }
    
  }
}
