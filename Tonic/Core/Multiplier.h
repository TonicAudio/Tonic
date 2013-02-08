//
//  Multiplier.h
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

#ifndef __TonicDemo__Multiplier__
#define __TonicDemo__Multiplier__

#include <iostream>
#include <vector>
#include "TonicFrames.h"
#include "Generator.h"
#include "FixedValue.h"

namespace Tonic{

  namespace Tonic_{

    class Multiplier_ : public Generator_{
      
    protected:
      TonicFrames *workSpace;
      vector<Generator> inputs;

    public:
      Multiplier_();
      ~Multiplier_();
      void in(Generator& inputSource);
      void tick( TonicFrames& frames);
      
      Generator & getInput(unsigned int index) { return inputs[index]; };
      unsigned int numInputs() { return inputs.size(); };
    };
    
    
    inline void Multiplier_::tick( TonicFrames& frames){
      
      if (workSpace->frames() == 0) {
        workSpace->resize(frames.frames(), frames.channels());
      }
      
      memset(&frames[0], 0, sizeof(TonicFloat) * frames.size());
      
      // for the first generator, store the value in the frame argument
      inputs[0].tick(frames);
      
      // for additional generators, use the workspace stkframes for tick, and multiply it into the frames argument
      for(int i = 1; i < inputs.size(); i++) {
        inputs[i].tick(*workSpace);
        frames *= *workSpace;
        
      }
      
    }

  }

  class Multiplier : public TemplatedGenerator<Tonic_::Multiplier_>{
  public:
    Multiplier in(Generator& inputSource){
      gen()->in(inputSource);
      return *this;
    }
    
    Generator & operator[](unsigned int index){
      return gen()->getInput(index);
    }
    
    unsigned int numInputs(){
      return gen()->numInputs();
    }

  };

  static Multiplier operator*(Generator a, Generator b){
    Multiplier mult;
    mult.in(a);
    mult.in(b);
    return mult;
  }
  
  static Multiplier operator*(float a, Generator b){
      return FixedValue(a) * b;
  }
  
  static Multiplier operator*(Generator a, float b){
      return a * FixedValue(b);
  }
  
  static Multiplier operator*(Generator a, Multiplier b){
    b.in(a);
    return b;
  }
  
  static Multiplier operator*(Multiplier a, Generator b){
    a.in(b);
    return a;
  }
  
  static Multiplier operator*(float a, Multiplier b){
    return FixedValue(a) * b;
  }
  
  static Multiplier operator*(Multiplier a, float b){
    return FixedValue(b) * a;
  }
  
  static Multiplier operator*(Multiplier a, Multiplier b){
    for (int i=0; i<b.numInputs(); i++){
      a.in(b[i]);
    }
    return a;
  }
}

#endif /* defined(__TonicDemo__Multiplier__) */
