//
//  Synth.h
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/23/13.
//
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

#ifndef ___013_1_23_melody__Synth__
#define ___013_1_23_melody__Synth__

#include <iostream>
#include <map>
#include "Source.h"
#include "ControlValue.h"

namespace Tonic{
  
  class Synth  : public Source{
    
  public:
    Synth();
    void                      tick( TonicFrames& frames );
    
    // It's quite conceivable that we'll want to move the messaging stuff up into Source
    ControlValue              registerMessage(string name, float value=0);
    void                      sendMessage(string name, float value=1);
    
  protected:
    
    std::map<string, ControlValue> parameters;
    
  };
  
  
}

#endif /* defined(___013_1_23_melody__Synth__) */
