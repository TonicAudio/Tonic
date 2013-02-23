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
#include "ControlValue.h"

namespace Tonic {
  namespace Tonic_{
  
    class FixedValue_ : public Generator_{

    private:
        
    protected:
      ControlGenerator valueGen;
        
    public:
        
        FixedValue_(float val){
          valueGen = ControlValue().setValue(val);
        }
        
        FixedValue_(){
          valueGen = ControlValue().setValue(0);
        }
         
        ~FixedValue_(){
        }
        
        inline void computeSynthesisBlock( const SynthesisContext & context ){
          
          float* buffStart = &synthesisBlock_[0];
          
          #ifdef USE_APPLE_ACCELERATE
          
          TonicFloat val = valueGen.getValue(context);
          vDSP_vfill( &val , buffStart, 1, synthesisBlock_.size());
          
          #else
          
          std::fill(buffStart, buffStart + synthesisBlock_.size(), valueGen.getValue(context));
          
          #endif
        }
      
        void setValue(ControlGenerator val){
          valueGen = val;
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
      gen()->setValue(ControlValue().setValue(val));
      return *this;
    }
    FixedValue& setValue(ControlGenerator val){
      gen()->setValue(val);
      return *this;
    }
  };
  
}

#endif /* defined(__TonicDemo__FixedValue__) */
