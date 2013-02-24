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
  
  void Mixer::addInput(BufferFiller* input)
  {
      // no checking for duplicates, maybe we should
    inputs.push_back(input);
    adder.in(input->getOutputGenerator());
  }
  
  void Mixer::removeInput(BufferFiller* input)
  {
    vector<BufferFiller*>::iterator it = inputs.begin();
    while (it != inputs.end()){
      if (*it == input){
        adder.remove((*it)->getOutputGenerator());
        inputs.erase(it);
        break;
      }
      it++;
    }
  }
  
} // Namespace Tonic
