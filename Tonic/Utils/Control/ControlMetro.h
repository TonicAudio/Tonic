//
//  ControlMetro.h
//  Tonic 
//
//  Created by Nick Donaldson on 3/10/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

/*+++++++++++++++++++++ License ++++++++++++++++++++

Use this code for whatever you want. There are NO 
RESTRICTIONS WHATSOVER. Modify it, repackage it, 
sell it, get rich from it, whatever. Go crazy. If 
you want to make mehappy, contribute to this 
project, but feel free to just use the code as a 
starting point for whatever you like.

Note that Tonic is heavily indebted to STK
https://ccrma.stanford.edu/software/stk/

++++++++++++++++++++++++++++++++++++++++++++++++++*/

// Metronome! Ouputs a "changed" status at a regular BPM interval

#ifndef __Tonic__ControlMetro__
#define __Tonic__ControlMetro__

#include "ControlGenerator.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlMetro_ : public ControlGenerator_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
      
    public:
      ControlMetro_();
      ~ControlMetro_();
      
    };
    
  }
  
  class ControlMetro : public TemplatedControlGenerator<Tonic_::ControlMetro_>{
    
  public:
      
  };
}

#endif /* defined(__Tonic__ControlMetro__) */


