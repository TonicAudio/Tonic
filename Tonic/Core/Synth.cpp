//
//  Synth.cpp
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

#include "Synth.h"
#include "SineWave.h"

namespace Tonic {

  Synth::Synth(){
    
  }

  inline void Synth::tick( TonicFrames& frames ){
    outputGen.tick(frames);
  };

  ControlValue  Synth::registerMessage(string name, float value){
    if (parameters.find(name)==parameters.end()) {
        parameters[name] = ControlValue().setValue(value);
    }
    return parameters[name];
  }
  
  void Synth::sendMessage(string name, float value){
    if (parameters.find(name)!=parameters.end()) {
      parameters[name].setValue(value);
    }else{
      error("message: " + name + " was not registered. You can register a message using Synth::registerMessage.");
    }
  }
  
}
