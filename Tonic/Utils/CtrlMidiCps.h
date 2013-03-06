//
//  CtrlMidiCps.h
//  TonicDemo
//
//  Created by Morgan Packard on 3/5/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef __TonicDemo__CtrlMidiCps__
#define __TonicDemo__CtrlMidiCps__

#include <iostream>
#include "ControlConditioner.h"

namespace Tonic{

namespace Tonic_{

  class CtrlMidiCps_ : public ControlConditioner_{
   
    ControlGeneratorStatus  computeStatus(const SynthesisContext_ & context);
    TonicFloat              computeValue(const SynthesisContext_ & context);
   
  };

}

  class CtrlMidiCps : public TemplatedControlConditioner<CtrlMidiCps, Tonic_::CtrlMidiCps_>{
    
  };

}


#endif /* defined(__TonicDemo__CtrlMidiCps__) */
