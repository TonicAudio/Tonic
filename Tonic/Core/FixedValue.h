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

#include "ControlValue.h"
#include "Generator.h"

namespace Tonic {
  
  namespace Tonic_{
  
    class FixedValue_ : public Generator_{
      
    protected:
      ControlGenerator valueGen;
        
    public:
        
      FixedValue_(float TonicFloat = 0);
      
      inline void computeSynthesisBlock( const SynthesisContext_ & context ){
        
        float* buffStart = &synthesisBlock_[0];
        
        ControlGeneratorOutput valueOutput = valueGen.tick(context);
        
        if (valueOutput.status == ControlGeneratorStatusHasChanged){
          
          #ifdef USE_APPLE_ACCELERATE
          
          vDSP_vfill( &valueOutput.value , buffStart, 1, synthesisBlock_.size());
          
          #else
          
          std::fill(buffStart, buffStart + synthesisBlock_.size(), valueOutput.value);
          
          #endif
        }
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
