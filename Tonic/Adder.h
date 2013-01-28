//
//  Adder.h
//  TonicDemo
//
//  Created by Morgan Packard on 1/27/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

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
      Adder_(int numChannels = 2){
        workSpace = new TonicFrames(kSynthesisBlockSize, numChannels);
      }
      
      ~Adder_(){
        delete workSpace;
      }
      
      // All generators must have the same number of channels, and this must match the number of channels 
      // in frames passed to tick.
      void in(Generator generator){
        audioSources.push_back( generator );
      }
      
      inline void tick( TonicFrames& frames ){
        
        TonicFloat *framesData =  &frames[0];
        
        memset(framesData, 0, sizeof(TonicFloat) * frames.size());
        
        for (int j =0; j < audioSources.size(); j++) {
            audioSources[j].tick(*workSpace);
               
            #ifdef USE_APPLE_ACCELERATE
                vDSP_vadd(&(*workSpace)[0], 1, &frames[0], 1, &frames[0], 1, frames.frames() * frames.channels());
            #else
                frames += *workSpace; // add each sample in frames to each sample in workspace
            #endif
        }
        
      }
    };
	
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
  
  
}


#endif /* defined(__TonicDemo__Adder__) */
