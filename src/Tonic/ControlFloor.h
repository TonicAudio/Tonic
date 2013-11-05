//
//  ControlFloor.h
//  TonicDemo
//
//  Created by Morgan Packard on 3/4/13.

//
// See LICENSE.txt for license and usage information.
//

#ifndef __TonicDemo__ControlFloor__
#define __TonicDemo__ControlFloor__

#include "ControlConditioner.h"

namespace Tonic{

  namespace Tonic_{

    class ControlFloor_ : public ControlConditioner_{
      
      inline void computeOutput(const SynthesisContext_ & context){
        output_.value = (int)input_.tick(context).value;
        output_.triggered = input_.tick(context).triggered;
      }
      
    };

  }

  class ControlFloor : public TemplatedControlConditioner<ControlFloor, Tonic_::ControlFloor_> {};

}

#endif /* defined(__TonicDemo__ControlFloor__) */
