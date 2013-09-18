//
//  ControlGenerator.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 2/12/13.

//

#ifndef TonicDemo_ControlGenerator_cpp
#define TonicDemo_ControlGenerator_cpp

#include "ControlGenerator.h"
#include "RampedValue.h"
#include <cmath>

namespace Tonic{
  namespace Tonic_{
    
    ControlGenerator_::ControlGenerator_() :
      lastFrameIndex_(0)
    {
    }

    ControlGenerator_::~ControlGenerator_()
    {
    }
    
    ControlGeneratorOutput ControlGenerator_::initialOutput(){
      // return output from passing in dummy context
      return tick(Tonic::DummyContext);
    }
    
    ControlGeneratorOutput ControlGenerator_::getLastOutput(){
      return output_;
    }
    
  }
  
  RampedValue ControlGenerator::smoothed(float length){
    return RampedValue( obj->initialOutput().value, length ).target(*this);
  }
  
  ControlGeneratorOutput ControlGenerator::getLastOutput(){
    return obj->getLastOutput();
  }
  
}

#endif
