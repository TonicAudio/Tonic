//
//  Reverb.cpp
//  Tonic
//
//  Created by Nick Donaldson on 5/4/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "Reverb.h"

#define TONIC_REVERB_MIN_TAPS 2
#define TONIC_REVERB_MAX_TAPS 20

#define TONIC_REVERB_MIN_WALL_DIST 3.0f // meters
#define TONIC_REVERB_MAX_WALL_DIST 30.0f // meters
#define TONIC_REVERB_SOS 340.0f // m/s, approximate
#define TONIC_REVERB_AIRDECAY -0.159 // db/m, approximate

#define TONIC_REVERB_FUDGE_AMT  0.1f // amount of randomization introduced to reflection times

namespace Tonic { namespace Tonic_{
  
  Reverb_::Reverb_(){
    
    setIsStereoOutput(true);
    
    preDelayLine_.initialize(0.0f, 0.25f);
    reflectDelayLine_.initialize(0.0f, 0.1f);
    
    workSpace_[0].resize(kSynthesisBlockSize, 1, 0);
    workSpace_[1].resize(kSynthesisBlockSize, 1, 0);
    
    preDelayTimeCtrlGen_ = ControlValue(0.01f);
    inputFiltBypasCtrlGen_ = ControlValue(false);
    densityCtrlGen_ = ControlValue(0.5);
    roomShapeCtrlGen_ = ControlValue(0.5);
    roomSizeCtrlGen_ = ControlValue(0.5);
    
    setInputLPFCutoffCtrlGen(ControlValue(20.f));
    setInputHPFCutoffCtrlGen(ControlValue(12000.f));
    
  }
  
  void Reverb_:: updateTapTimes(const SynthesisContext_ & context)
  {
    ControlGeneratorOutput densityOutput = densityCtrlGen_.tick(context);
    ControlGeneratorOutput shapeOutput = roomShapeCtrlGen_.tick(context);
    ControlGeneratorOutput sizeOutput = roomSizeCtrlGen_.tick(context);
    
    if (densityOutput.status == ControlGeneratorStatusHasChanged ||
        shapeOutput.status == ControlGeneratorStatusHasChanged ||
        sizeOutput.status == ControlGeneratorStatusHasChanged)
    {
      
      reflectTapTimes_.clear();
      reflectTapScale_.clear();
      
      // compute base round-trip times from listener to wall, based on shape and size values
      TonicFloat shape = clamp(shapeOutput.value, 0.f, 1.f);
      TonicFloat size = clamp(shapeOutput.value, 0.f, 1.f);

      TonicFloat wDist1 = map(size, 0.f, 1.f, TONIC_REVERB_MIN_WALL_DIST, TONIC_REVERB_MAX_WALL_DIST, true);
      TonicFloat wDist2 = map(size * (1.1f - shape), 0.f, 1.f, TONIC_REVERB_MIN_WALL_DIST, TONIC_REVERB_MAX_WALL_DIST, true);

      unsigned int nTaps = (unsigned int)map(densityOutput.value, 0.f, 1.f, TONIC_REVERB_MIN_TAPS, TONIC_REVERB_MAX_TAPS, true);
      
      for (unsigned int i=0; i<nTaps; i++){
        
        TonicFloat dist = (i % 2 == 0 ? wDist1 : wDist2) * (1.0f + randomFloat(-TONIC_REVERB_FUDGE_AMT, TONIC_REVERB_FUDGE_AMT));
        
        reflectTapTimes_.push_back( dist/TONIC_REVERB_SOS );
        reflectTapScale_.push_back( dBToLin(dist * TONIC_REVERB_AIRDECAY) );
      }
      
    }
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
