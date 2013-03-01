//
//  ControlValue.h
//  TonicDemo
//
//  Created by Morgan Packard on 2/12/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef __TonicDemo__ControlValue__
#define __TonicDemo__ControlValue__

#include "ControlGenerator.h"

namespace Tonic {

  namespace Tonic_{

    class ControlValue_ : public ControlGenerator_{
      public:
        ControlValue_();
        ~ControlValue_();

        void setValue(float value);
      
        // Get current value directly
        TonicFloat getValue() { return value_;};
      
      protected:
            
        ControlGeneratorStatus computeStatus(const SynthesisContext_ & context);
        TonicFloat computeValue(const SynthesisContext_ & context);
      
        TonicFloat  value_;
        bool        hasChanged_;
      
    };
  }
  
  
  class ControlValue : public TemplatedControlGenerator<Tonic_::ControlValue_>{
    
    public:
    
    ControlValue(float value = 0){
      gen()->setValue(value);
    }
    
    ControlValue & setValue(float value)
    {
      gen()->setValue(value);
      return *this;
    }
    
    // for higher-level access - no
    float getValue(){
      return gen()->getValue();
    }

  };

}

#endif /* defined(__TonicDemo__ControlValue__) */
