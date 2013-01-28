//
//  Synth.h
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/23/13.
//
//

#ifndef ___013_1_23_melody__Synth__
#define ___013_1_23_melody__Synth__

#include <iostream>
#include "Tonic.h"
#include "Generator.h"
#include "TonicFrames.h"

namespace Tonic{
  
class Synth  : public Generator{

private:
  int                     synthBufferReadPosition;
  TonicFrames             outputFrames;

protected:

  Generator              outputGen;
  
public:
  Synth();
  void                    tick( TonicFrames& frames );
  void                    fillBufferOfFloats(float *outData,  UInt32 numFrames, int numChannels);
  template<typename T>T&  a(T*);
};
  
}

#endif /* defined(___013_1_23_melody__Synth__) */
