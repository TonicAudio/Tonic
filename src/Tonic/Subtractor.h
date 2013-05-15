//
//  Subtractor.h
//  Tonic 
//
//  Created by Morgan Packard on 4/19/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__Subtractor__
#define __Tonic__Subtractor__

#include "Generator.h"
#include "FixedValue.h"

namespace Tonic {
  
  namespace Tonic_ {

    class Subtractor_ : public Generator_{
      
    protected:
    
      Generator left;
      Generator right;
      TonicFrames workSpace;
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    public:
      
      Subtractor_();

      void setLeft(Generator arg){
        left = arg;
      }
      
      void setRight(Generator arg){
        right = arg;
      }
      
    };
    
    inline void Subtractor_::computeSynthesisBlock(const SynthesisContext_ &context){
      left.tick(outputFrames_, context);
      right.tick(workSpace, context);
      outputFrames_ -= workSpace;
    }
    
  }
  
  class Subtractor : public TemplatedGenerator<Tonic_::Subtractor_>{
    
  public:
  
    createGeneratorSetters(Subtractor, left, setLeft);
    createGeneratorSetters(Subtractor, right, setRight);

  };
  
  
  static Subtractor operator - (Generator a, Generator b){
    Subtractor sub;
    sub.left(a);
    sub.right(b);
    return sub;
  }
  
  static Subtractor operator - (float a, Generator b){
    Subtractor sub;
    sub.left(FixedValue(a));
    sub.right(b);
    return sub;
  }
  
  static Subtractor operator - (Generator a, float b){
    Subtractor sub;
    sub.left(a);
    sub.right(FixedValue(b));
    return sub;
  }
  
  // Subtract a Generatator and a ControlGenerator
  
  static Subtractor operator - (Generator a, ControlGenerator b){
    return a - FixedValue().setValue(b);
  }
  
  static Subtractor operator - (ControlGenerator a, Generator b){
    return FixedValue().setValue(a) - b;
  }
  
}

#endif /* defined(__Tonic__Subtractor__) */


