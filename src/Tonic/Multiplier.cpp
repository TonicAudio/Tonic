//
//  Multiplier.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 1/25/13.

//
// See LICENSE.txt for license and usage information.
//


#include "Multiplier.h"

namespace Tonic { namespace Tonic_{
  
  Multiplier_::Multiplier_(){
    workSpace.resize(kSynthesisBlockSize, 1, 0);
  }
  
  Multiplier_::~Multiplier_(){
  }
  
  void Multiplier_::input(Generator generator){
    lockMutex();
    inputs.push_back(generator);
    if ( generator.isStereoOutput() && !this->isStereoOutput() ){
      this->setIsStereoOutput(true);
      workSpace.resize(kSynthesisBlockSize, 2, 0);
    }
    unlockMutex();
  }
  
}}
