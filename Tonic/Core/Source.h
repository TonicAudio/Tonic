//
//  Source.h
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

#ifndef __TonicDemo__Source__
#define __TonicDemo__Source__

#include <iostream>
#include <map>
#include "Generator.h"

//! Source is the base class for any generator expected to produce output for a buffer fill.
/*!
 Sources provide a high-level interface for combinations of generators, and can be used to fill
 arbitraryly large buffers.
  
 Subclasses to include mixer, channel, synth, etc.
*/
namespace Tonic{
  
  class Source : Generator {
    
  private:
    int           bufferReadPosition;
    TonicFrames   outputFrames;
    
  protected:
    
    Generator     outputGen;
    
  public:
    Source();
    void          tick( TonicFrames& frames );
    void          fillBufferOfFloats(float *outData,  UInt32 numFrames, int numChannels);
    inline const Generator & getOutputGenerator() { return outputGen; };
  };

  // ------------------------------
  //
  // Adding factory/registration for easy wrappers and instantiation
  //
  // http://stackoverflow.com/questions/582331/is-there-a-way-to-instantiate-objects-from-a-string-holding-their-class-name
  //
  // Concrete source subclasses should implement a static registration struct to be registered by default:
  //
  // In MySource.h:
  //
  // private:
  //  static SourceRegister<MySource> reg;
  //
  // In Source.cpp
  //  SourceRegister<MySource> MySource::reg("MySource");
  //
  // -----------------------------
  
  template<typename T> Source * createSource() { return new T; }
  
  struct SourceFactory {
    
    typedef std::map<std::string, Source*(*)()> map_type;
    
    static Source * createInstance(std::string const& s) {
      map_type::iterator it = getMap()->find(s);
      if(it == getMap()->end()){
        string synthsList = "";
      
        for(it = getMap()->begin(); it != getMap()->end(); it++){
          synthsList = synthsList + it->first + "\n";
        }
      
        error("Error creating sysnth. Synth named \"" + s + "\" not found. Existing registered synths are: \n" + synthsList);

        return 0;
      }
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
  struct SourceRegister : SourceFactory {
    SourceRegister(std::string const& s) {
      getMap()->insert(std::make_pair(s, &createSource<T>));
    }
  };
  
}

#endif /* defined(__TonicDemo__Source__) */
