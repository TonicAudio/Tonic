//
//  Arithmetic.cpp
//  Tonic
//
//  Created by Morgan Packard on 1/27/13.

//
// See LICENSE.txt for license and usage information.
//


#include "Arithmetic.h"

namespace Tonic{ namespace Tonic_ {

  // -----------------------------------------
  //                 ADDER
  // -----------------------------------------
  
  Adder_::Adder_(){
    workSpace.resize(kSynthesisBlockSize, 1, 0);
  }
  
  void Adder_::input(Generator generator){
    inputs.push_back( generator );
    if ( generator.isStereoOutput() && !this->isStereoOutput() ){
      this->setIsStereoOutput(true);
      workSpace.resize(kSynthesisBlockSize, 2, 0);
    }
  }
  
  // -----------------------------------------
  //                SUBTRACTOR
  // -----------------------------------------
  
  Subtractor_::Subtractor_(){
    workSpace.resize(kSynthesisBlockSize, 1, 0);
  }
  
  // -----------------------------------------
  //                MULTIPLIER
  // -----------------------------------------
  
  Multiplier_::Multiplier_(){
    workSpace.resize(kSynthesisBlockSize, 1, 0);
  }
  
  void Multiplier_::input(Generator generator){
    inputs.push_back(generator);
    if ( generator.isStereoOutput() && !this->isStereoOutput() ){
      this->setIsStereoOutput(true);
      workSpace.resize(kSynthesisBlockSize, 2, 0);
    }
  }
  
  // -----------------------------------------
  //                DIVIDER
  // -----------------------------------------
  
  Divider_::Divider_(){
    workSpace.resize(kSynthesisBlockSize, 1, 0);
  }
  
}}