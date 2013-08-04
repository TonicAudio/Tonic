//
//  ControlMidiToFreq.h
//  TonicDemo
//
//  Created by Morgan Packard on 3/5/13.

//
// See LICENSE.txt for license and usage information.
//

#ifndef __TonicDemo__ControlMidiToFreq__
#define __TonicDemo__ControlMidiToFreq__

#include <iostream>
#include "ControlConditioner.h"

namespace Tonic{

  namespace Tonic_{

    class ControlMidiToFreq_ : public ControlConditioner_{
     
      void computeOutput(const SynthesisContext_ & context){
        ControlGeneratorOutput inputOut = input_.tick(context);
        output_.triggered = inputOut.triggered;
        output_.value = mtof( inputOut.value );
      }
      
    };

  }

  class ControlMidiToFreq : public TemplatedControlConditioner<ControlMidiToFreq, Tonic_::ControlMidiToFreq_> {};

}


#endif /* defined(__TonicDemo__ControlMidiToFreq__) */
