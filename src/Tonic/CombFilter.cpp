//
//  CombFilter.cpp
//  Tonic
//
//  Created by Nick Donaldson on 5/4/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "CombFilter.h"

namespace Tonic { namespace Tonic_{
  
  CombFilter_::CombFilter_(){
    delayTimeFrames_.resize(kSynthesisBlockSize, 1, 0);
  }
  
  void CombFilter_::initialize(float initialDelayTime, float maxDelayTime){
    delayLine_.initialize(initialDelayTime, maxDelayTime);
  }
  
} // Namespace Tonic_
  
  FFCombFilter::FFCombFilter(float initialDelayTime, float maxDelayTime)
  {
    gen()->initialize(initialDelayTime, maxDelayTime);
    delayTime(initialDelayTime);
    scaleFactor(0.5f);
  }
  
  FBCombFilter::FBCombFilter(float initialDelayTime, float maxDelayTime)
  {
    gen()->initialize(initialDelayTime, maxDelayTime);
    delayTime(initialDelayTime);
    scaleFactor(0.5f);
  }

  
} // Namespace Tonic
