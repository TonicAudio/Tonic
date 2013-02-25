//
//  ControlMultiplier.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/25/13.
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
      
      void in( ControlGenerator input );
      
      ControlGenerator & getInput(unsigned int index) { return inputs[index]; };
      unsigned int numInputs() { return inputs.size(); };
      
    protected:
      
      bool computeHasChanged( const SynthesisContext_ & context );
      TonicFloat computeValue( const SynthesisContext_ & context );
      
      vector<ControlGenerator> inputs;
      
    };
    
  }
  
  class ControlMultiplier : public TemplatedControlGenerator<Tonic_::ControlMultiplier_>{
    
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
  
  static ControlMultiplier operator*(ControlGenerator a, ControlGenerator b){
    ControlMultiplier adder;
    adder.in(a);
    adder.in(b);
    return adder;
  }
  
  static ControlMultiplier operator*(ControlGenerator a, float b){
    ControlMultiplier adder;
    adder.in(a);
    adder.in(ControlValue(b));
    return adder;
  }
  
  static ControlMultiplier operator*(float a, ControlGenerator b){
    ControlMultiplier adder;
    adder.in(ControlValue(a));
    adder.in(ControlGenerator(b));
    return adder;
  }
  
  static ControlMultiplier operator*(ControlMultiplier a, ControlGenerator b){
    a.in(b);
    return a;
  }
  
  static ControlMultiplier operator*(ControlGenerator a, ControlMultiplier b){
    b.in(a);
    return b;
  }
  
  static ControlMultiplier operator*(ControlMultiplier a, float b){
    a.in(ControlValue(b));
    return a;
  }
  
  static ControlMultiplier operator*(float a, ControlMultiplier b){
    b.in(ControlValue(a));
    return b;
  }
  
  static ControlMultiplier operator*(ControlMultiplier a, ControlMultiplier b){
    for (unsigned int i=0; i<b.numInputs(); i++){
      a.in(b[i]);
    }
    return a;
  }
}


#endif /* defined(__Tonic__ControlMultiplier__) */


