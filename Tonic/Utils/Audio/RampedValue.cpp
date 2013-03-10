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
    len_(0),
    target_(0),
    last_(0),
    inc_(0)
  {
    // shouldn't need to set this. See https://github.com/morganpackard/Tonic/issues/6
    // still, good to give default values to the relevant generators
    setTargetGen(ControlValue().setValue(0));
    setLengthGen(ControlValue().setValue(0.05));
    setValueGen(ControlValue().setValue(0));
  }
  
  RampedValue_::~RampedValue_(){
    
  }
  
  
  bool RampedValue_::isFinished(){
    return finished_;
  }

} // Namespace Tonic_
  
  RampedValue::RampedValue(TonicFloat startValue, TonicFloat initLength ){
    value(startValue);
    length(initLength);
  }
  
  //! Set target value
  RampedValue & RampedValue::target( TonicFloat target ){
    ControlValue v = ControlValue().setValue(target);
    gen()->lockMutex();
    gen()->setTargetGen(v);
    gen()->unlockMutex();
    return *this;
  }
  
  RampedValue & RampedValue::target( ControlGenerator target ){
    gen()->lockMutex();
    gen()->setTargetGen(target);
    gen()->unlockMutex();
    return *this;
  }
  
  //! Set length before reaching target value, in ms
  RampedValue & RampedValue::length( TonicFloat length ){
    ControlValue v = ControlValue().setValue(length);
    gen()->lockMutex();
    gen()->setLengthGen(v);
    gen()->unlockMutex();
    return *this;
  }
  
  RampedValue & RampedValue::length( ControlGenerator lengthMs ){
    gen()->lockMutex();
    gen()->setLengthGen(lengthMs);
    gen()->unlockMutex();
    return *this;
  }
  
  //! Go to value immediately
  RampedValue & RampedValue::value( TonicFloat value){
    ControlValue v = ControlValue().setValue(value);
    gen()->lockMutex();
    gen()->setValueGen(v);
    gen()->unlockMutex();
    return *this;
  }
  
  RampedValue & RampedValue::value( ControlGenerator value){
    gen()->lockMutex();
    gen()->setValueGen(value);
    gen()->unlockMutex();
    return *this;
  }
  
  bool RampedValue::isFinished(){
    return gen()->isFinished();
  }
  
} // Namespace Tonic
