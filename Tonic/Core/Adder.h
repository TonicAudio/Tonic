//
//  Adder.h
//  TonicDemo
//
//  Created by Morgan Packard on 1/27/13.
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

#ifndef __TonicDemo__Adder__
#define __TonicDemo__Adder__

#include <iostream>
#include "Generator.h"
#include <vector>
#include "FixedValue.h"

namespace Tonic {
  namespace Tonic_{
      
    class Adder_ : public Generator_ {
    protected:
      std::vector<Generator> audioSources;
      TonicFrames *workSpace;
      TonicFrames *lastFrames; // in case we want to grab these and examine them
    public:
      
      // Not sure how this could ever be called with an argument, since it's always allocated
      // via a templated container...
      Adder_(int numChannels = 2);
      ~Adder_();
      
      // All generators must have the same number of channels, and this must match the number of channels 
      // in frames passed to tick.
      void in(Generator generator);
      inline void tick( TonicFrames& frames );
    };
    
    inline void Adder_::tick( TonicFrames& frames ){
      
      TonicFloat *framesData =  &frames[0];
      
      memset(framesData, 0, sizeof(TonicFloat) * frames.size());
      
      for (int j =0; j < audioSources.size(); j++) {
        audioSources[j].tick(*workSpace);
        frames += *workSpace; // add each sample in frames to each sample in workspace
      }
      
      
    }
	
  }
  
  class Adder : public TemplatedGenerator<Tonic_::Adder_>{
  public:
    Adder in(Generator input){
      gen()->in(input);
      return *this;
    }
    
  };
  
  static Adder operator + (Generator a, Generator b){
    Adder add;
    add.in(a);
    add.in(b);
    return add;
  }
  
  
  static Adder operator + (float a, Generator b){
    Adder add;
    add.in(FixedValue(a));
    add.in(b);
    return add;
  }
  
  
  static Adder operator + (Generator a, float b){
    Adder add;
    add.in(a);
    add.in(FixedValue(b));
    return add;
  }

  // TODO: Handle stringing adders together
  
}


#endif /* defined(__TonicDemo__Adder__) */
