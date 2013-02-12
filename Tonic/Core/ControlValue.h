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

namespace Tonic {
  class ControlValue{
    public:
      ControlValue();
      ~ControlValue();
      bool hasChanged();
      void setHasChanged(bool flagVal=true);
      bool getValue();
      void setValue(float);

    private:
      bool mHasChanged;
      float mValue;
  };
}

#endif /* defined(__TonicDemo__ControlValue__) */
