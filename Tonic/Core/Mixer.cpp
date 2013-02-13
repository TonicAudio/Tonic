//
//  Mixer.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/9/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "Mixer.h"

namespace Tonic {
  
  Mixer::Mixer(){
    outputGen = adder;
  }
  
  void Mixer::addInput(Source* input)
  {
      // no checking for duplicates, maybe we should
    sources.push_back(input);
    adder.in(input->getOutputGenerator());
  }
  
  void Mixer::removeInput(Source* input)
  {
    vector<Source*>::iterator it = sources.begin();
    while (it != sources.end()){
      if (*it == input){
        adder.remove((*it)->getOutputGenerator());
        sources.erase(it);
        break;
      }
      it++;
    }
  }
  
} // Namespace Tonic
