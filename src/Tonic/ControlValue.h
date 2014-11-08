//
//  ControlValue.h
//  Tonic
//
//  Created by Morgan Packard on 2/12/13.

//
// See LICENSE.txt for license and usage information.
//

#ifndef TONIC_CONTROLVALUE_H
#define TONIC_CONTROLVALUE_H

#include "ControlGenerator.h"

namespace Tonic {

  namespace Tonic_{

    class ControlValue_ : public ControlGenerator_{
    
    public:
      
        ControlValue_();
      
        inline void setValue(float value){
          value_ = value;
          changed_ = true;
        }
            
        // Get current value directly
        TonicFloat getValue() { return value_;};
      
      protected:
            
        void computeOutput(const SynthesisContext_ & context);
      
        TonicFloat  value_;
        bool        changed_;
      
    };
    
    inline void ControlValue_::computeOutput(const SynthesisContext_ & context){
      output_.triggered =  (changed_ || context.forceNewOutput);
      changed_ = context.forceNewOutput; // if new output forced, don't reset changed status until next tick
      output_.value = value_;
    }
  }
  
  
  class ControlValue : public TemplatedControlGenerator<Tonic_::ControlValue_>{
    
    public:
    
    ControlValue(TonicFloat value = 0){
      gen()->setValue(value);
    }
    
    ControlValue & value(TonicFloat value)
    {
      gen()->setValue(value);
      return *this;
    }
    
    // for higher-level access - doesn't affect tick state
    float getValue(){
      return gen()->getValue();
    }

  };

}

#endif /* defined(__TonicDemo__ControlValue__) */
