//
//  FixedValue.h
//  TonicDemo
//
//  Created by Morgan Packard on 1/27/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

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
            std::fill(buffStart, buffStart + frames.size(), mValue);
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
