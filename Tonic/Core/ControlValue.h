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
        bool hasChanged();
        void setHasChanged(bool flagVal=true);
        TonicFloat getValue();
        void setValue(float);

      private:
        bool mHasChanged;
        float mValue;
      
    };
  }
  
  class ControlValue : public TemplatedControlGenerator<Tonic_::ControlValue_>{
    public:
      ControlValue & setValue(float value)
      {
        gen()->setValue(value);
        return *this;
      }
    
    ControlValue & setHasChanged(bool flagVal){
      gen()->setHasChanged(flagVal);
      return *this;
    }
  };

}

#endif /* defined(__TonicDemo__ControlValue__) */
