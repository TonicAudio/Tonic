//
//  Adder.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 1/27/13.

//
// See LICENSE.txt for license and usage information.
//


#include "Adder.h"

namespace Tonic{ namespace Tonic_ {

  Adder_::Adder_(){
    workSpace.resize(kSynthesisBlockSize, 1, 0);
  }
  
  Adder_::~Adder_(){
  }
  
  void Adder_::input(Generator generator){
    inputs.push_back( generator );
    if ( generator.isStereoOutput() && !this->isStereoOutput() ){
      this->setIsStereoOutput(true);
      workSpace.resize(kSynthesisBlockSize, 2, 0);
    }
  }
  
}}