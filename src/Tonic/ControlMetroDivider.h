//
//  ControlMetroDivider.h
//  Tonic 
//
//  Created by Nick Donaldson on 5/31/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ControlMetroDivider__
#define __Tonic__ControlMetroDivider__

#include "ControlConditioner.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlMetroDivider_ : public ControlConditioner_ {
      
    protected:
      
      void computeOutput(const SynthesisContext_ & context);
      
      ControlGenerator divisonsGen_;
      ControlGenerator offsetGen_;
      
      unsigned int offset_;
      unsigned int tickCounter_;
      
    public:

      ControlMetroDivider_();
      
      void setDivisionsGen( ControlGenerator gen ) { divisonsGen_ = gen; }
      void setOffsetGen( ControlGenerator gen ) { offsetGen_ = gen; }
      
    };
    
  }
  
  class ControlMetroDivider : public TemplatedControlConditioner<ControlMetroDivider, Tonic_::ControlMetroDivider_>
  {
  public:
    
    createControlGeneratorSetters(ControlMetroDivider, divisions, setDivisionsGen);
    createControlGeneratorSetters(ControlMetroDivider, offset, setOffsetGen);
    

  };
}

#endif /* defined(__Tonic__ControlMetroDivider__) */


