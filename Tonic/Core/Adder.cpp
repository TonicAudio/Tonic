//
//  Adder.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 1/27/13.
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

#include "Adder.h"
#include "Generator.h"
#include <vector>
#include "FixedValue.h"

namespace Tonic{ namespace Tonic_ {

  Adder_::Adder_(int numChannels){
    workSpace.resize(kSynthesisBlockSize, numChannels);
  }
  
  Adder_::~Adder_(){
  }
  
  void Adder_::in(Generator generator){
    lockMutex();
    inputs.push_back( generator );
    unlockMutex();
  }
  
  void Adder_::remove(Generator generator){
    
    vector<Generator>::iterator it = inputs.begin();
    while (it != inputs.end()){
      if (*it == generator){
        lockMutex();
        inputs.erase(it);
        unlockMutex();
        break;
      }
      it++;
    }
  }
}}