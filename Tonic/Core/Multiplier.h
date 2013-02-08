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
      inline void tick( TonicFrames& frames);
      
    };

  }

  class Multiplier : public TemplatedGenerator<Tonic_::Multiplier_>{
  public:
    Multiplier in(Generator& inputSource){
      gen()->in(inputSource);
      return *this;
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

}

#endif /* defined(__TonicDemo__Multiplier__) */
