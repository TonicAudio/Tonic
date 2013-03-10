//
//  Adder.h
//  TonicDemo
//
//  Created by Morgan Packard on 1/27/13.
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

#ifndef __Tonic__Adder__
#define __Tonic__Adder__

#include "Generator.h"

namespace Tonic {
  namespace Tonic_{
      
    class Adder_ : public Generator_ {
    protected:
      std::vector<Generator> inputs;
      TonicFrames workSpace;

    public:
      
      Adder_();
      ~Adder_();
      
      void in(Generator generator);
      
      Generator & getInput(unsigned int index) { return inputs[index]; };
      unsigned int numInputs() { return inputs.size(); };
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    };
    
    inline void Adder_::computeSynthesisBlock( const SynthesisContext_ &context ){
      
      TonicFloat *framesData =  &synthesisBlock_[0];
      
      memset(framesData, 0, sizeof(TonicFloat) * synthesisBlock_.size());
      
      for (int j =0; j < inputs.size(); j++) {
        inputs[j].tick(workSpace, context);
        synthesisBlock_ += workSpace; // add each sample in frames to each sample in workspace
      }
      
    }
	
  }
  
  class Adder : public TemplatedGenerator<Tonic_::Adder_>{
  public:
    
    Adder in(Generator input){
      gen()->in(input);
      return *this;
    }
    
    Generator & operator[](unsigned int index){
      return gen()->getInput(index);
    }
    
    unsigned int numInputs(){
      return gen()->numInputs();
    }
    
  };
  
  static Adder operator + (Generator a, Generator b){
    Adder add;
    add.in(a);
    add.in(b);
    return add;
  }
  
  
  static Adder operator + (float a, Generator b){
    Adder add;
    add.in(FixedValue(a));
    add.in(b);
    return add;
  }
  
  
  static Adder operator + (Generator a, float b){
    Adder add;
    add.in(a);
    add.in(FixedValue(b));
    return add;
  }

  static Adder operator + (Adder a, Generator b){
    a.in(b);
    return a;
  }
  
  static Adder operator + (Generator a, Adder b){
    b.in(a);
    return b;
  }
  
  static Adder operator + (Adder a, float b){
    a.in(FixedValue(b));
    return a;
  }
  
  static Adder operator + (float a, Adder b){
    b.in(FixedValue(a));
    return b;
  }
  
  static Adder operator + (Adder a, Adder b){
    for (int i=0; i<b.numInputs(); i++){
      a.in(b[i]);
    }
    return a;
  }
  
}


#endif /* defined(__TonicDemo__Adder__) */
