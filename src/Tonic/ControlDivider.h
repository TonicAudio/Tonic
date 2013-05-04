//
//  ControlDivider.h
//  Tonic 
//
//  Created by Morgan Packard on 4/19/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ControlDivider__
#define __Tonic__ControlDivider__

#include "ControlGenerator.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlDivider_ : public ControlGenerator_{
      
    protected:
      ControlGenerator left;
      ControlGenerator right;
      void computeOutput(const SynthesisContext_ & context);
      
    public:
      ControlDivider_();
      ~ControlDivider_();
      void setLeft(ControlGenerator arg){
        left = arg;
      }
      void setRight(ControlGenerator arg){
        right = arg;
      }
      
    };
    
  }
  
  class ControlDivider : public TemplatedControlGenerator<Tonic_::ControlDivider_>{
    
  public:
    
    createControlGeneratorSetters(ControlDivider, left, setLeft);
    createControlGeneratorSetters(ControlDivider, right, setRight);

  };
  
  static ControlDivider operator/(ControlGenerator a, ControlGenerator b){
    ControlDivider divider;
    divider.left(a);
    divider.right(b);
    return divider;
  }
  
  static ControlDivider operator/(ControlGenerator a, float b){
    ControlDivider divider;
    divider.left(a);
    divider.right(ControlValue(b));
    return divider;
  }
  
  static ControlDivider operator/(float a, ControlGenerator b){
    ControlDivider divider;
    divider.left(ControlValue(a));
    divider.right(ControlGenerator(b));
    return divider;
  }
  
}

#endif /* defined(__Tonic__ControlDivider__) */


