//
//  ControlValue.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 2/12/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "ControlValue.h"

namespace Tonic {
  
  ControlValue::ControlValue():mHasChanged(false),mValue(0){}
  ControlValue::~ControlValue(){}
  
  bool ControlValue::hasChanged(){
    return mHasChanged;
  }
  
  void ControlValue::setHasChanged(bool flagVal){
    mHasChanged = flagVal;
  }
  
  bool ControlValue::getValue(){
    return mValue;
  }
  
  void ControlValue::setValue(float value){
    mValue = value;
    mHasChanged = true;
  }
}
