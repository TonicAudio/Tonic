//
//  Multiplier.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 1/25/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

/*+++++++++++++++++++++ License ++++++++++++++++++++

Use this code for whatever you want. There are NO 
RESTRICTIONS WHATSOVER. Modify it, repackage it, 
sell it, get rich from it, whatever. Go crazy. If 
you want to make mehappy, contribute to this 
project, but feel free to just use the code as a 
starting point for whatever you like.

Note that Tonic is heavily indebted to STK
https://ccrma.stanford.edu/software/stk/

++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "Multiplier.h"

namespace Tonic { namespace Tonic_{
  
  Multiplier_::Multiplier_(){
    workSpace = new TonicFrames(kSynthesisBlockSize, 2);
  }
  
  Multiplier_::~Multiplier_(){
    delete workSpace;
  }
  
  void Multiplier_::in(Generator& inputSource){
    inputs.push_back(inputSource);
  }
  
}}