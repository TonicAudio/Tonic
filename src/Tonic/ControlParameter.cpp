//
//  ControlParameter.cpp
//  Tonic
//
//  Created by Nick Donaldson on 5/14/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "ControlParameter.h"

namespace Tonic { namespace Tonic_{
  
  ControlParameter_::ControlParameter_() :
    name_(""),
    displayName_(""),
    min_(0),
    max_(1),
    type_(ControlParameterTypeContinuous),
    logBase_(2.f),
    isLogarithmic_(false)
  {
    
  }
  
  void ControlParameter_::setNormalizedValue(TonicFloat normVal){
    if (isLogarithmic_){
      
    }
    else{
      setValue(map(normVal, 0.f, 1.f, min_, max_, true));
    }
  }
  
  TonicFloat ControlParameter_::getNormalizedValue()
  {
    
  }
  
  
} // Namespace Tonic_
  
  ControlParameter::ControlParameter(string name){
    gen()->setName(name);
  }

  string              getName();
  ControlParameter &  name(string name);
  
  string              getDisplayName();
  ControlParameter &  displayName(string displayName);
  
  TonicFloat          getValue();
  ControlParameter &  value(TonicFloat value);
  
  ControlParameter &  min(TonicFloat min);
  TonicFloat          getMin();
  
  ControlParameter &  max();
  TonicFloat          getMax();
  
  ControlParameter &    type(ControlParameterType type);
  ControlParameterType  getType();
  
  ControlParameter &  logarithmic(bool isLogarithmic);
  bool                getIsLogarithmic();
  
  ControlParameter &  logBase(TonicFloat logBase);
  TonicFloat          getLogBase();
  
} // Namespace Tonic
