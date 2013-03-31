//
//  Multiplier.h
//  TonicDemo
//
//  Created by Morgan Packard on 1/25/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__Multiplier__
#define __Tonic__Multiplier__

#include <vector>
#include "Generator.h"

namespace Tonic{

  namespace Tonic_{

    class Multiplier_ : public Generator_{
      
    protected:
      TonicFrames workSpace;
      vector<Generator> inputs;

    public:
      Multiplier_();
      ~Multiplier_();
      void in(Generator& generator);
      void computeSynthesisBlock( const SynthesisContext_ & context );
      
      Generator & getInput(unsigned int index) { return inputs[index]; };
      unsigned int numInputs() { return inputs.size(); };
    };
    
    
    inline void Multiplier_::computeSynthesisBlock( const SynthesisContext_ & context ){
      
      memset(&synthesisBlock_[0], 0, sizeof(TonicFloat) * synthesisBlock_.size());
      
      // for the first generator, store the value in the block
      inputs[0].tick(synthesisBlock_, context);
      
      // for additional generators, use the workspace stkframes for tick, and multiply it into the frames argument
      for(int i = 1; i < inputs.size(); i++) {
        inputs[i].tick(workSpace, context);
        synthesisBlock_ *= workSpace;
      }
      
    }

  }

  class Multiplier : public TemplatedGenerator<Tonic_::Multiplier_>{
  public:
    Multiplier in(Generator& inputSource){
      gen()->in(inputSource);
      return *this;
    }
    
    Generator & operator[](unsigned int index){
      return gen()->getInput(index);
    }
    
    unsigned int numInputs(){
      return gen()->numInputs();
    }

  };

  static Multiplier operator*(Generator a, Generator b){
    Multiplier mult;
    mult.in(a);
    mult.in(b);
    return mult;
  }
  
  static Multiplier operator*(float a, Generator b){
      return FixedValue(a) * b;
  }
  
  static Multiplier operator*(Generator a, float b){
      return a * FixedValue(b);
  }
  
  static Multiplier operator*(Generator a, Multiplier b){
    b.in(a);
    return b;
  }
  
  static Multiplier operator*(Multiplier a, Generator b){
    a.in(b);
    return a;
  }
  
  static Multiplier operator*(float a, Multiplier b){
    return FixedValue(a) * b;
  }
  
  static Multiplier operator*(Multiplier a, float b){
    return FixedValue(b) * a;
  }
  
  static Multiplier operator*(Multiplier a, Multiplier b){
    for (int i=0; i<b.numInputs(); i++){
      a.in(b[i]);
    }
    return a;
  }
}

#endif /* defined(__TonicDemo__Multiplier__) */
