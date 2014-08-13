//
//  ControlDbToLinear.h
//  Tonic 
//
//  Created by Nick Donaldson on 3/24/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_CONTROLDBTOLINEAR_H
#define TONIC_CONTROLDBTOLINEAR_H

#include "ControlConditioner.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlDbToLinear_ : public ControlConditioner_{
      
    protected:
      
      void computeOutput(const SynthesisContext_ & context);
      
    };
    
    inline void ControlDbToLinear_::computeOutput(const SynthesisContext_ & context){
      
      ControlGeneratorOutput inputOutput = input_.tick(context);
      output_.triggered = inputOutput.triggered;
      if (inputOutput.triggered){
        output_.value = dBToLin(inputOutput.value);
      }
      
    }
    
  }
  
  class ControlDbToLinear : public TemplatedControlConditioner<ControlDbToLinear, Tonic_::ControlDbToLinear_>{
    
  public:

  };
}

#endif


