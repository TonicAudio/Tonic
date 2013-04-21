//
//  ControlSubtractor.h
//  Tonic 
//
//  Created by Morgan Packard on 4/19/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ControlSubtractor__
#define __Tonic__ControlSubtractor__

#include "ControlGenerator.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlSubtractor_ : public ControlGenerator_{
      
    protected:
      ControlGenerator left;
      ControlGenerator right;
      void computeOutput(const SynthesisContext_ & context);
      
    public:
      ControlSubtractor_();
      ~ControlSubtractor_();
      void setLeft(ControlGenerator arg){
        left = arg;
      }
      void setRight(ControlGenerator arg){
        right = arg;
      }
    };
    
  }
  
  class ControlSubtractor : public TemplatedControlGenerator<Tonic_::ControlSubtractor_>{
    
  public:
  
    createControlGeneratorSetters(ControlSubtractor, left, setLeft);
    createControlGeneratorSetters(ControlSubtractor, right, setRight);

  };

  static ControlSubtractor operator-(ControlGenerator a, ControlGenerator b){
    ControlSubtractor subtractor;
    subtractor.left(a);
    subtractor.right(b);
    return subtractor;
  }
  
  static ControlSubtractor operator-(ControlGenerator a, float b){
    ControlSubtractor subtractor;
    subtractor.left(a);
    subtractor.right(ControlValue(b));
    return subtractor;
  }
  
  static ControlSubtractor operator-(float a, ControlGenerator b){
    ControlSubtractor subtractor;
    subtractor.left(ControlValue(a));
    subtractor.right(ControlGenerator(b));
    return subtractor;
  }
  


}

#endif /* defined(__Tonic__ControlSubtractor__) */


