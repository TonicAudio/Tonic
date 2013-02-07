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
    workSpace.resize(0);
  }
  
  void Multiplier_::in(Generator& inputSource){
    inputs.push_back(inputSource);
  }

  
  void Multiplier_::tick( TonicFrames& frames){
  
    if (workSpace.frames() == 0) {
      workSpace.resize(frames.frames(), frames.channels());
    }

    memset(&frames[0], 0, sizeof(TonicFloat) * frames.size());
    
    // for the first generator, store the value in the frame argument
    inputs[0].tick(frames);
    
    // for additional generators, use the workspace stkframes for tick, and multiply it into the frames argument
    for(int i = 1; i < inputs.size(); i++) {
      inputs[i].tick(workSpace);
      frames *= workSpace;

    }

  }
  
}}
