//
//  ControlValue.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 2/12/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "ControlValue.h"

namespace Tonic {
  
  namespace Tonic_{
  
    ControlValue_::ControlValue_():mHasChanged(false),mValue(0){}
    ControlValue_::~ControlValue_(){}
    
    bool ControlValue_::hasChanged(){
      return mHasChanged;
    }
    
    void ControlValue_::setHasChanged(bool flagVal){
      mHasChanged = flagVal;
    }
    
    TonicFloat ControlValue_::getValue(){
      // TODO: Until we figure out a better way to manage multiple instances of
      // ControlGenerators, this should also reset hasChanged()
      mHasChanged = false;
      return mValue;
    }
    
    void ControlValue_::setValue(float value){
      mValue = value;
      mHasChanged = true;
    }
  
  }
  
}
