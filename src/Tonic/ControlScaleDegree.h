//
//  ControlScaleDegree.h
//  Tonic 
//
//  Created by Morgan Packard on 8/14/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ControlScaleDegree__
#define __Tonic__ControlScaleDegree__

#include "ControlGenerator.h"
#include "ControlSwitcher.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlScaleDegree_ : public ControlGenerator_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
      
    public:
      ControlScaleDegree_();
      ~ControlScaleDegree_();
      
    };
    
  }
  
  class ControlScaleDegree : public TemplatedControlGenerator<Tonic_::ControlScaleDegree_>{
    
  public:

  };
}

#endif /* defined(__Tonic__ControlScaleDegree__) */


