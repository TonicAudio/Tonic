//
//  ControlDbToLinear.h
//  Tonic 
//
//  Created by Nick Donaldson on 3/24/13.
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

#ifndef __Tonic__ControlDbToLinear__
#define __Tonic__ControlDbToLinear__

#include "ControlConditioner.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlDbToLinear_ : public ControlConditioner_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
      
    public:
      ControlDbToLinear_();
      ~ControlDbToLinear_();
      
    };
    
  }
  
  class ControlDbToLinear : public TemplatedControlConditioner<ControlDbToLinear, Tonic_::ControlDbToLinear_>{
    
  public:

  };
}

#endif /* defined(__Tonic__ControlDbToLinear__) */


