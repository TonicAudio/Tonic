  //
//  FixedValue.h
//  TonicDemo
//
//  Created by Morgan Packard on 1/27/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef __TonicDemo__FixedValue__
#define __TonicDemo__FixedValue__

#include "ControlValue.h"
#include "Generator.h"

namespace Tonic {
  
  namespace Tonic_{
  
    class FixedValue_ : public Generator_{
      
    protected:
      
      ControlGenerator valueGen;
      
      void computeSynthesisBlock( const SynthesisContext_ & context );

        
    public:
        
      FixedValue_(float TonicFloat = 0);
    
      void setValue(ControlGenerator val){
        valueGen = val;
      }

    };
    
    inline void FixedValue_::computeSynthesisBlock( const SynthesisContext_ & context ){
      
      float* buffStart = &outputFrames_[0];
      
      ControlGeneratorOutput valueOutput = valueGen.tick(context);
      
      if (valueOutput.triggered){
        
#ifdef USE_APPLE_ACCELERATE
        
        vDSP_vfill( &valueOutput.value , buffStart, 1, outputFrames_.size());
        
#else
        
        std::fill(buffStart, buffStart + outputFrames_.size(), valueOutput.value);
        
#endif
      }
    }

  }
  
  class FixedValue : public TemplatedGenerator<Tonic_::FixedValue_>{
  public:
    FixedValue(){}
    FixedValue(float val){
      setValue(val);
    }
    FixedValue& setValue(float val){
      gen()->setValue(ControlValue(val));
      return *this;
    }
    FixedValue& setValue(ControlGenerator val){
      gen()->setValue(val);
      return *this;
    }
  };
  
}

#endif /* defined(__TonicDemo__FixedValue__) */
