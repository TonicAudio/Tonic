//
//  FilterUtils.cpp
//  TonicDemo
//
//  Created by Nick Donaldson on 2/20/13.

#include "FilterUtils.h"

namespace Tonic {
  
  Biquad::Biquad(){
    memset(coef_, 0, 5 * sizeof(TonicFloat));
    inputVec_.resize(kSynthesisBlockSize + 4, 1, 0);
    outputVec_.resize(kSynthesisBlockSize + 4, 1, 0);
  }
  
}