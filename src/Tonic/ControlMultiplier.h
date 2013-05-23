//
//  ControlMultiplier.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/25/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ControlMultiplier__
#define __Tonic__ControlMultiplier__

#include <vector>
#include "ControlGenerator.h"
#include "ControlValue.h"

using std::vector;

namespace Tonic {
  
  namespace Tonic_ {
    
    class ControlMultiplier_ : public ControlGenerator_{
      
    public:
      
      void input( ControlGenerator input );
      
      ControlGenerator getInput(unsigned int index) { return inputs[index]; };
      unsigned int numInputs() { return inputs.size(); };
      
    protected:
      
      void computeOutput( const SynthesisContext_ & context );
      
      vector<ControlGenerator> inputs;
      
    };
    
  }
  
  class ControlMultiplier : public TemplatedControlGenerator<Tonic_::ControlMultiplier_>{
    
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
  
  static ControlMultiplier operator*(ControlGenerator a, ControlGenerator b){
    ControlMultiplier mult;
    mult.input(a);
    mult.input(b);
    return mult;
  }
  
  static ControlMultiplier operator*(ControlGenerator a, float b){
    ControlMultiplier mult;
    mult.input(a);
    mult.input(ControlValue(b));
    return mult;
  }
  
  static ControlMultiplier operator*(float a, ControlGenerator b){
    ControlMultiplier mult;
    mult.input(ControlValue(a));
    mult.input(ControlGenerator(b));
    return mult;
  }
}


#endif /* defined(__Tonic__ControlMultiplier__) */


