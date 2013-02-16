//
//  RampedValue.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/7/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "RampedValue.h"

namespace Tonic { namespace Tonic_{
  
  RampedValue_::RampedValue_() :
    finished_(true),
    count_(0),
    len_(2205),
    target_(0),
    last_(0),
    inc_(0)
  {
    // shouldn't need to set this. See https://github.com/morganpackard/Tonic/issues/6
    // still, good to give default values to the relevant generators
    setTargetGen(ControlValue().setValue(0));
    setLengthMsGen(ControlValue().setValue(0.5f));
    setValueGen(ControlValue().setValue(0));
  }
  
  RampedValue_::~RampedValue_(){
    
  }

} // Namespace Tonic_
  
  
  //! Set target value
  RampedValue & RampedValue::target( TonicFloat target ){
    ControlValue v = ControlValue().setValue(target);
    gen()->setTargetGen(v);
    return *this;
  }
  
  RampedValue & RampedValue::target( ControlGenerator target ){
    gen()->setTargetGen(target);
    return *this;
  }
  
  //! Set length before reaching target value, in ms
  RampedValue & RampedValue::lengthMs( TonicFloat lengthMs ){
    ControlValue v = ControlValue().setValue(lengthMs);
    gen()->setLengthMsGen(v);
    return *this;
  }
  
  RampedValue & RampedValue::lengthMs( ControlGenerator lengthMs ){
    gen()->setLengthMsGen(lengthMs);
    return *this;
  }
  
  //! Go to value immediately
  RampedValue & RampedValue::value( TonicFloat value){
    ControlValue v = ControlValue().setValue(value);
    gen()->setValueGen(v);
    return *this;
  }
  
  RampedValue & RampedValue::value( ControlGenerator value){
    gen()->setValueGen(value);
    return *this;
  }
  
} // Namespace Tonic
