//
//  ControlTrigger.h
//  Tonic 
//
//  Created by Morgan Packard on 3/31/13.
//
// See LICENSE.txt for license and usage information.

#ifndef TONIC_CONTROLTRIGGER_H
#define TONIC_CONTROLTRIGGER_H

#include "ControlGenerator.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlTrigger_ : public ControlGenerator_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
      bool doTrigger;
      
    public:
      ControlTrigger_();
      void trigger(float value);
      
    };
    
    inline void ControlTrigger_::computeOutput(const SynthesisContext_ & context){
      output_.triggered = doTrigger;
      doTrigger = false;
    }
    
  }
  
  /*!
    Status changes to ControlGeneratorStatusHasChanged when trigger is called
  */
  
  
  class ControlTrigger : public TemplatedControlGenerator<Tonic_::ControlTrigger_>{
    
  public:
  
  void trigger(float value = 1);

  };
}

#endif


