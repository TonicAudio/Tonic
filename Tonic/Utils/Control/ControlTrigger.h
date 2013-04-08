//
//  ControlTrigger.h
//  Tonic 
//
//  Created by Morgan Packard on 3/31/13.
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

#ifndef __Tonic__ControlTrigger__
#define __Tonic__ControlTrigger__

#include "ControlGenerator.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlTrigger_ : public ControlGenerator_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
      bool doTrigger;
      
    public:
      ControlTrigger_();
      ~ControlTrigger_();
      void trigger();
      
    };
    
  }
  
  /*!
    Status changes to ControlGeneratorStatusHasChanged when trigger is called
  */
  
  
  class ControlTrigger : public TemplatedControlGenerator<Tonic_::ControlTrigger_>{
    
  public:
  
  void trigger();

  };
}

#endif /* defined(__Tonic__ControlTrigger__) */


