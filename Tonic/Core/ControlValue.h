//
//  ControlValue.h
//  TonicDemo
//
//  Created by Morgan Packard on 2/12/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef __TonicDemo__ControlValue__
#define __TonicDemo__ControlValue__

#include <iostream>
#include "ControlGenerator.h"

namespace Tonic {

  namespace Tonic_{

    class ControlValue_ : public ControlGenerator_{
      public:
        ControlValue_();
        ~ControlValue_();
      
        bool hasChanged(const SynthesisContext_ & context);
        TonicFloat getValue(const SynthesisContext_ & context);
      
        // version without context argument, for use in ramped() shortcut, among other things
        TonicFloat getValue();

        void setHasChanged(bool flagVal=true);
        void setValue(float);

      private:
        bool          mHasChanged;
        bool          mLastHasChanged;
        float         mValue;
        unsigned long mLastFrameIndex;
    };
  }
  
  class RampedValue;
  
  class ControlValue : public TemplatedControlGenerator<Tonic_::ControlValue_>{
    
    public:
    
    ControlValue(float value = 0){
      setValue(value);
    }
    
    inline ControlValue & setValue(float value)
    {
      gen()->setValue(value);
      return *this;
    }
    
    inline ControlValue & setHasChanged(bool flagVal){
      gen()->setHasChanged(flagVal);
      return *this;
    }

    // shortcut for creating ramped value
    RampedValue ramped(float lenMs = 50);
  };

}

#endif /* defined(__TonicDemo__ControlValue__) */
