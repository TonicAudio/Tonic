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
#include "Tonic.h"
#include "Generator.h"
#include "TonicFrames.h"
#include <map>
#include "ControlValue.h"

namespace Tonic{
  
class Synth  : public Generator{

private:
  int                       synthBufferReadPosition;
  TonicFrames               outputFrames;

protected:
  
  std::map<string, ControlValue> parameters;
  Generator                 outputGen;
  
public:
  Synth();
  void                      tick( TonicFrames& frames );
  void                      fillBufferOfFloats(float *outData,  UInt32 numFrames, int numChannels);
  void                      setOutputGen(Generator);
  ControlValue              registerMessage(string name, float value=0);
  void                      sendMessage(string name, float value=1);
};
  
}

#endif /* defined(___013_1_23_melody__Synth__) */
