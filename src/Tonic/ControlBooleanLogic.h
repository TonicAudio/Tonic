//
//  ControlBooleanLogic.h
//  Tonic 
//
//  Created by Nick Donaldson on 5/31/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ControlBooleanLogic__
#define __Tonic__ControlBooleanLogic__

#include "ControlConditioner.h"

namespace Tonic {
  
  namespace Tonic_ {

    // TODO: Base class for all logical operators
    class ControlGreaterThan_ : public ControlConditioner_ {
      
    protected:
      
      void computeOutput(const SynthesisContext_ & context);
      
      ControlGenerator rhsGen_;
      
    public:
      
      ControlGreaterThan_();
      void setRhsGen( ControlGenerator gen ) { rhsGen_ = gen; }
      
    };
    
  }
  
  class ControlGreaterThan : public TemplatedControlConditioner<ControlGreaterThan, Tonic_::ControlGreaterThan_>{
    
  public:
    
    createControlGeneratorSetters(ControlGreaterThan, rhs, setRhsGen);

  };
}

#endif /* defined(__Tonic__ControlBooleanLogic__) */


