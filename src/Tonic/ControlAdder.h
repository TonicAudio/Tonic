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
      
      void in( ControlGenerator input );
      
      ControlGenerator & getInput(unsigned int index) { return inputs[index]; };
      unsigned int numInputs() { return inputs.size(); };
      
    protected:
      
      void computeOutput( const SynthesisContext_ & context );
      
      vector<ControlGenerator> inputs;
      
    };
    
  }
  
  class ControlAdder : public TemplatedControlGenerator<Tonic_::ControlAdder_>{
    
  public:
    
    void in( ControlGenerator input ){
      gen()->in( input );
    }
    
    ControlGenerator & operator[](unsigned int index){
      return gen()->getInput(index);
    }
    
    unsigned int numInputs(){
      return gen()->numInputs();
    }
  };
  
  static ControlAdder operator+(const ControlGenerator & a, const ControlGenerator & b){
    ControlAdder adder;
    adder.in(a);
    adder.in(b);
    return adder;
  }
  
  static ControlAdder operator+(const ControlGenerator & a, float b){
    ControlAdder adder;
    adder.in(a);
    adder.in(ControlValue(b));
    return adder;
  }
  
  static ControlAdder operator+(float a, const ControlGenerator & b){
    ControlAdder adder;
    adder.in(ControlValue(a));
    adder.in(ControlGenerator(b));
    return adder;
  }
  
  static ControlAdder operator+(ControlAdder & a, const ControlGenerator & b){
    a.in(b);
    return a;
  }
  
  static ControlAdder operator+(const ControlGenerator & a, ControlAdder & b){
    b.in(a);
    return b;
  }
  
  static ControlAdder operator+(ControlAdder & a, float b){
    a.in(ControlValue(b));
    return a;
  }
  
  static ControlAdder operator+(float a, ControlAdder & b){
    b.in(ControlValue(a));
    return b;
  }
  
  static ControlAdder operator+(ControlAdder & a, ControlAdder & b){
    for (unsigned int i=0; i<b.numInputs(); i++){
      a.in(b[i]);
    }
    return a;
  }
}

#endif /* defined(__Tonic__ControlAdder__) */


