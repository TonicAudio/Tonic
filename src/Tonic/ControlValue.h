//
//  ControlValue.h
//  TonicDemo
//
//  Created by Morgan Packard on 2/12/13.

//

#ifndef __TonicDemo__ControlValue__
#define __TonicDemo__ControlValue__

#include "ControlGenerator.h"

namespace Tonic {

  namespace Tonic_{

    class ControlValue_ : public ControlGenerator_{
    
    public:
      
        ControlValue_();

        void setValue(float value);
      
        // Get current value directly
        TonicFloat getValue() { return value_;};
      
      protected:
            
        void computeOutput(const SynthesisContext_ & context);
      
        TonicFloat  value_;
        bool        hasChanged_;
      
    };
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


// This allows anything that knows about ControlValues to also use the operator overloads
// Probably should move the implementation of the operator overloads into the .cpp file.
#include "ControlMultiplier.h"
#include "ControlAdder.h"

#endif /* defined(__TonicDemo__ControlValue__) */
