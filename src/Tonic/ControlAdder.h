//
//  ControlAdder.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/24/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ControlAdder__
#define __Tonic__ControlAdder__

#include <vector>
#include "ControlGenerator.h"
#include "ControlValue.h"

using std::vector;

namespace Tonic {
  
  namespace Tonic_ {
    
    class ControlAdder_ : public ControlGenerator_{
      
    public:
      
      void input( ControlGenerator input );
      
      ControlGenerator getInput(unsigned int index) { return inputs[index]; };
      unsigned int numInputs() { return inputs.size(); };
      
    protected:
      
      void computeOutput( const SynthesisContext_ & context );
      
      vector<ControlGenerator> inputs;
      
    };
    
  }
  
  class ControlAdder : public TemplatedControlGenerator<Tonic_::ControlAdder_>{
    
  public:
    
    void input( ControlGenerator input ){
      gen()->input( input );
    }
    
    ControlGenerator operator[](unsigned int index){
      return gen()->getInput(index);
    }
    
    unsigned int numInputs(){
      return gen()->numInputs();
    }
  };
  
  static ControlAdder operator+(ControlGenerator a, ControlGenerator b){
    ControlAdder adder;
    adder.input(a);
    adder.input(b);
    return adder;
  }
  
  static ControlAdder operator+(ControlGenerator a, float b){
    ControlAdder adder;
    adder.input(a);
    adder.input(ControlValue(b));
    return adder;
  }
  
  static ControlAdder operator+(float a, ControlGenerator b){
    ControlAdder adder;
    adder.input(ControlValue(a));
    adder.input(ControlGenerator(b));
    return adder;
  }
  
}

#endif /* defined(__Tonic__ControlAdder__) */


