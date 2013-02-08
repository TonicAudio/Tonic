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
  
// ------------------------------
//
// Adding factory/registration for easy wrappers and instance access throughout an application
//
// http://stackoverflow.com/questions/582331/is-there-a-way-to-instantiate-objects-from-a-string-holding-their-class-name
//
// Synth subclasses should implement a static registration struct to be registered by default:
//
// In MySynth.h:
//
// private:
//  static SynthRegister<MySynth> reg;
//
// In MySynth.cpp
//  SynthRegister<MySynth> MySynth::reg("MySynth");
//
// -----------------------------
  
template<typename T> Synth * createSynth() { return new T; }

struct SynthFactory {
  
  typedef std::map<std::string, Synth*(*)()> map_type;
  
  static Synth * createInstance(std::string const& s) {
    map_type::iterator it = getMap()->find(s);
    if(it == getMap()->end())
      return 0;
    return it->second();
  }
  
protected:
  static map_type * getMap() {
    // never delete'ed. (exist until program termination)
    // because we can't guarantee correct destruction order
    if(!map) { map = new map_type; }
    return map;
  }
  
private:
  static map_type * map;
};
  
template<typename T>
struct SynthRegister : SynthFactory {
  SynthRegister(std::string const& s) {
    getMap()->insert(std::make_pair(s, &createSynth<T>));
  }
};
  
}

#endif /* defined(___013_1_23_melody__Synth__) */
