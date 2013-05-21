//
//  Divider.h
//  Tonic 
//
//  Created by Morgan Packard on 4/19/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__Divider__
#define __Tonic__Divider__

#include "Generator.h"
#include "FixedValue.h"

namespace Tonic {
  
  namespace Tonic_ {

    class Divider_ : public Generator_{
      
    protected:
      Generator left;
      Generator right;
      TonicFrames workSpace;
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    public:
      Divider_();
      ~Divider_();
      
      void setLeft(Generator arg){
        left = arg;
      }
      
      void setRight(Generator arg){
        right = arg;
      }
      
    };
    
    inline void Divider_::computeSynthesisBlock(const SynthesisContext_ &context){
      left.tick(outputFrames_, context);
      right.tick(workSpace, context);
      outputFrames_ /= workSpace;

    }
    
  }
  
  class Divider : public TemplatedGenerator<Tonic_::Divider_>{
    
  public:
    createGeneratorSetters(Divider, left, setLeft);
    createGeneratorSetters(Divider, right, setRight);

  };
  
    
  
  static Divider operator / (Generator a, Generator b){
    Divider div;
    div.left(a);
    div.right(b);
    return div;
  }
  
  static Divider operator / (float a, Generator b){
    Divider div;
    div.left(FixedValue(a));
    div.right(b);
    return div;
  }
  
  static Divider operator / (Generator a, float b){
    Divider div;
    div.left(a);
    div.right(FixedValue(b));
    return div;
  }
  
  // Subtract a Generatator and a ControlGenerator
  
  static Divider operator / (Generator a, ControlGenerator b){
    return a / FixedValue().setValue(b);
  }
  
  static Divider operator / (ControlGenerator a, Generator b){
    return FixedValue().setValue(a) / b;
  }
  
  
  
}

#endif /* defined(__Tonic__Divider__) */


