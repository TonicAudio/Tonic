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

#define TONIC_REVERB_FUDGE_AMT  0.05f // amount of randomization introduced to reflection times

// Number of FF comb filters
#define  TONIC_REVERB_N_COMBS 8
#define  TONIC_REVERB_MIN_COMB_TIME  0.01f
#define  TONIC_REVERB_MAX_COMB_TIME  0.1f
#define  TONIC_REVERB_STEREO_SPREAD 0.001f

namespace Tonic { namespace Tonic_{
  
  // Changing these will change the character of the late-stage reverb. Tune as you wish.
  static const TonicFloat reverbCombTimeScales_[TONIC_REVERB_N_COMBS] = {1.17, 1.12, 1.02, 0.97, 0.95, 0.88, 0.84, 0.82};
  
  Reverb_::Reverb_(){
    
    setIsStereoOutput(true);
    
    preDelayLine_.initialize(0.0f, 0.25f);
    reflectDelayLine_.initialize(0.0f, 0.1f);
    
    workspaceFrames_[0].resize(kSynthesisBlockSize, 1, 0);
    workspaceFrames_[1].resize(kSynthesisBlockSize, 1, 0);
    preOutputFrames_[0].resize(kSynthesisBlockSize, 1, 0);
    preOutputFrames_[1].resize(kSynthesisBlockSize, 1, 0);
    
    preDelayTimeCtrlGen_ = ControlValue(0.01f);
    inputFiltBypasCtrlGen_ = ControlValue(false);
    densityCtrlGen_ = ControlValue(0.5);
    roomShapeCtrlGen_ = ControlValue(0.5);
    roomSizeCtrlGen_ = ControlValue(0.5);
    decayTimeCtrlGen_ = ControlValue(1.0f);
    stereoWidthCtrlGen_ = ControlValue(0.5f);
    
    setInputLPFCutoffCtrlGen(ControlValue(12000.f));
    setInputHPFCutoffCtrlGen(ControlValue(20.f));
    
    for (unsigned int i=0; i<TONIC_REVERB_N_COMBS; i++){
      
      ControlValue delayL = ControlValue(0.01f);
      ControlValue delayR = ControlValue(0.01f);
      ControlValue scaleL = ControlValue(0.5);
      ControlValue scaleR = ControlValue(0.5);
      combFilterDelayTimes_[TONIC_LEFT].push_back(delayL);
      combFilterDelayTimes_[TONIC_RIGHT].push_back(delayR);
      combFilterScaleFactors_[TONIC_LEFT].push_back(scaleL);
      combFilterScaleFactors_[TONIC_RIGHT].push_back(scaleR);
      combFilters_[TONIC_LEFT].push_back(FilteredFBCombFilter6(0.01f, 0.125f).delayTime(delayL).scaleFactor(scaleL));
      combFilters_[TONIC_RIGHT].push_back(FilteredFBCombFilter6(0.01f, 0.125f).delayTime(delayR).scaleFactor(scaleR));
    }
    
    setDecayLPFCtrlGen(ControlValue(12000.f));
    setDecayHPFCtrlGen(ControlValue(20.f));
    
  }
  
  void Reverb_:: updateDelayTimes(const SynthesisContext_ & context)
  {
    ControlGeneratorOutput densityOutput = densityCtrlGen_.tick(context);
    ControlGeneratorOutput shapeOutput = roomShapeCtrlGen_.tick(context);
    ControlGeneratorOutput sizeOutput = roomSizeCtrlGen_.tick(context);
    ControlGeneratorOutput decayOutput = decayTimeCtrlGen_.tick(context);
    
    if (densityOutput.status == ControlGeneratorStatusHasChanged ||
        shapeOutput.status == ControlGeneratorStatusHasChanged ||
        sizeOutput.status == ControlGeneratorStatusHasChanged)
    {
      
      reflectTapTimes_.clear();
      reflectTapScale_.clear();
      
      // compute base round-trip times from listener to wall, based on shape and size values
      TonicFloat shape = clamp(shapeOutput.value, 0.f, 1.f);
      TonicFloat size = clamp(sizeOutput.value, 0.f, 1.f);

      TonicFloat wDist1 = map(size, 0.f, 1.f, TONIC_REVERB_MIN_WALL_DIST, TONIC_REVERB_MAX_WALL_DIST, true);
      TonicFloat wDist2 = map(size * (1.1f - shape), 0.f, 1.f, TONIC_REVERB_MIN_WALL_DIST, TONIC_REVERB_MAX_WALL_DIST, true);

      unsigned int nTaps = (unsigned int)map(densityOutput.value, 0.f, 1.f, TONIC_REVERB_MIN_TAPS, TONIC_REVERB_MAX_TAPS, true);
      
      for (unsigned int i=0; i<nTaps; i++){
        
        TonicFloat dist = (i % 2 == 0 ? wDist1 : wDist2) * (1.0f + randomFloat(-TONIC_REVERB_FUDGE_AMT, TONIC_REVERB_FUDGE_AMT));
        
        reflectTapTimes_.push_back( dist/TONIC_REVERB_SOS );
        reflectTapScale_.push_back( dBToLin(dist * TONIC_REVERB_AIRDECAY) );
      }
      
    }
    
    // if decay or room size have changed, need to update comb times/scales
    if (sizeOutput.status == ControlGeneratorStatusHasChanged ||
        decayOutput.status == ControlGeneratorStatusHasChanged)
    {
      
      TonicFloat decayTime = decayOutput.value;
      TonicFloat baseCombDelayTime = map(sizeOutput.value, 0.f, 1.f, TONIC_REVERB_MIN_COMB_TIME, TONIC_REVERB_MAX_COMB_TIME, true);
      
      for (unsigned int i=0; i<TONIC_REVERB_N_COMBS; i++){
        
        TonicFloat scaledDelayTime = reverbCombTimeScales_[i % TONIC_REVERB_N_COMBS] * baseCombDelayTime;
        combFilterDelayTimes_[TONIC_LEFT][i].setValue(scaledDelayTime);
        combFilterDelayTimes_[TONIC_RIGHT][i].setValue(scaledDelayTime+TONIC_REVERB_STEREO_SPREAD);
        combFilterScaleFactors_[TONIC_LEFT][i].setValue( powf(10.f, (-3.0f * scaledDelayTime / decayTime)) );
        combFilterScaleFactors_[TONIC_RIGHT][i].setValue( powf(10.f, (-3.0f * (scaledDelayTime+TONIC_REVERB_STEREO_SPREAD) / decayTime)) );

      }
    }
    
  }
  
  void Reverb_::setDecayLPFCtrlGen( ControlGenerator gen )
  {
    for (unsigned int i=0; i<TONIC_REVERB_N_COMBS; i++){
      combFilters_[TONIC_LEFT][i].lowpassCutoff(gen);
      combFilters_[TONIC_RIGHT][i].lowpassCutoff(gen);
    }
  }
  
  void Reverb_::setDecayHPFCtrlGen( ControlGenerator gen )
  {
    for (unsigned int i=0; i<TONIC_REVERB_N_COMBS; i++){
      combFilters_[TONIC_LEFT][i].highpassCutoff(gen);
      combFilters_[TONIC_RIGHT][i].highpassCutoff(gen);
    }
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
