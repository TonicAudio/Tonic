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
      
    public:
      Divider_();
      ~Divider_();
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
      void setLeft(Generator arg){
        left = arg;
      }
      
      void setRight(Generator arg){
        right = arg;
      }
      
    };
    
    inline void Divider_::computeSynthesisBlock(const SynthesisContext_ &context){
      left.tick(synthesisBlock_, context);
      right.tick(workSpace, context);
      synthesisBlock_ /= workSpace;

    }
    
  }
  
  class Divider : public TemplatedGenerator<Tonic_::Divider_>{
    
  public:
    createGeneratorSetters(Divider, left, setLeft);
    createGeneratorSetters(Divider, right, setRight);

  };
}

#endif /* defined(__Tonic__Divider__) */


