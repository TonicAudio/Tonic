//
//  FixedValue.h
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

#ifndef __TonicDemo__FixedValue__
#define __TonicDemo__FixedValue__

#include <iostream>
#include "TonicFrames.h"
#include "Generator.h"

namespace Tonic {
  namespace Tonic_{
  
    class FixedValue_ : public Generator_{

    private:
        
    protected:
        
    public:
        
        float mValue;
        
        FixedValue_(float val) : mValue(val){
        }
        
        FixedValue_(){
        }
         
        ~FixedValue_(){
        }
        
        inline void tick( TonicFrames& frames){
            float* buffStart = &frames[0];
        #ifdef USE_APPLE_ACCELERATE
            vDSP_vfill(&mValue, buffStart, 1, frames.size());
        #else
            std::fill(buffStart, buffStart + frames.size(), mValue);
        #endif
        }

    };

  }
  
  class FixedValue : public TemplatedGenerator<Tonic_::FixedValue_>{
  public:
    FixedValue(){}
    FixedValue(float val){
      setValue(val);
    }
    FixedValue& setValue(float val){
      gen()->mValue = val;
      return *this;
    }
  };
  
}

#endif /* defined(__TonicDemo__FixedValue__) */
