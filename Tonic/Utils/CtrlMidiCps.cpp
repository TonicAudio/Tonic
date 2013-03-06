//
//  CtrlMidiCps.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 3/5/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "CtrlMidiCps.h"


namespace Tonic{

  namespace Tonic_{
  
    ControlGeneratorStatus  CtrlMidiCps_::computeStatus(const SynthesisContext_ & context){
      return input_.tick(context).status;
    }
    
    TonicFloat CtrlMidiCps_::computeValue(const SynthesisContext_ & context){
      float midi = input_.tick(context).value;
      return mtof( midi );
    }
    
  }
}
