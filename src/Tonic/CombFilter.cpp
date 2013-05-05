//
//  CombFilter.cpp
//  Tonic
//
//  Created by Nick Donaldson on 5/4/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "CombFilter.h"

namespace Tonic { namespace Tonic_{
  
  FFCombFilter_::FFCombFilter_(){

  }
  
  FFCombFilter_::~FFCombFilter_(){
    
  }
  
  void FFCombFilter_::initialize(float initialDelayTime, float maxDelayTime){
    delayLine_.initialize(initialDelayTime, maxDelayTime);
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
