//
//  Mixer.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/9/13.
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

//! A mixer is like an adder but acts as a source and allows dynamic removal

#ifndef __Tonic__Mixer__
#define __Tonic__Mixer__

#include <vector>
#include "Adder.h"
#include "Source.h"

using std::vector;

namespace Tonic {

  class Mixer : public Source{
  
  private:
    vector<Source*> sources;
    Adder adder;
    
  public:
    
    Mixer();
    void addInput(Source* input);
    void removeInput(Source* input);
    void tick( TonicFrames& frames );
  };
}

#endif /* defined(__Tonic__Mixer__) */


