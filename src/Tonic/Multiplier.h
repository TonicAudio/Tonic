//
//  Multiplier.h
//  TonicDemo
//
//  Created by Morgan Packard on 1/25/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__Multiplier__
#define __Tonic__Multiplier__

#include "Generator.h"

namespace Tonic{

  namespace Tonic_{

    class Multiplier_ : public Generator_{
      
    protected:
      TonicFrames workSpace;
      vector<Generator> inputs;
      
      void computeSynthesisBlock( const SynthesisContext_ & context );

    public:
      Multiplier_();
      ~Multiplier_();
      
      void input(Generator generator);
      
      Generator getInput(unsigned int index) { return inputs[index]; };
      unsigned int numInputs() { return inputs.size(); };
    };
    
    
    inline void Multiplier_::computeSynthesisBlock( const SynthesisContext_ & context ){
      
      memset(&outputFrames_[0], 0, sizeof(TonicFloat) * outputFrames_.size());
      
      // for the first generator, store the value in the block
      inputs[0].tick(outputFrames_, context);
      
      // for additional generators, use the workspace stkframes for tick, and multiply it into the frames argument
      for(int i = 1; i < inputs.size(); i++) {
        inputs[i].tick(workSpace, context);
        outputFrames_ *= workSpace;
      }
      
    }

  }

  class Multiplier : public TemplatedGenerator<Tonic_::Multiplier_>{
  public:
    Multiplier input(Generator inputSource){
      gen()->input(inputSource);
      return *this;
    }
    
    Generator operator[](unsigned int index){
      return gen()->getInput(index);
    }
    
    unsigned int numInputs(){
      return gen()->numInputs();
    }

  };

  static Multiplier operator*(Generator a, Generator b){
    Multiplier mult;
    mult.input(a);
    mult.input(b);
    return mult;
  }
  
  static Multiplier operator*(float a, Generator b){
      return FixedValue(a) * b;
  }
  
  static Multiplier operator*(Generator a, float b){
      return a * FixedValue(b);
  }
  
  
  // Multiply a Generatator and a ControlGenerator
  
  static Multiplier operator * (Generator a, ControlGenerator b){
    return a * FixedValue().setValue(b);
  }
  
  static Multiplier operator * (ControlGenerator a, Generator b){
    return FixedValue().setValue(a) * b;
  }
  
  // Multiply an Multiplier and a ControlGenerator
  
  static Multiplier operator * (Multiplier a, ControlGenerator b){
    return a * FixedValue().setValue(b);
  }
  
  static Multiplier operator * (ControlGenerator a, Multiplier b){
    return FixedValue().setValue(a) * b;
  }
}

#endif /* defined(__TonicDemo__Multiplier__) */
