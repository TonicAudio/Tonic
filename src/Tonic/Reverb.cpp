//
//  Reverb.cpp
//  Tonic
//
//  Created by Nick Donaldson on 5/4/13.
//

#include "Reverb.h"

#define TONIC_REVERB_MIN_TAPS 2
#define TONIC_REVERB_MAX_TAPS 20

#define TONIC_REVERB_MIN_WALL_DIST 3.0f // meters
#define TONIC_REVERB_MAX_WALL_DIST 10.0f // meters
#define TONIC_REVERB_SOS 340.0f // m/s, approximate
#define TONIC_REVERB_AIRDECAY -0.159 // db/m, approximate

#define TONIC_REVERB_FUDGE_AMT  0.05f // amount of randomization introduced to reflection times

// Number of FF comb filters per channel
#define  TONIC_REVERB_N_COMBS 8
#define  TONIC_REVERB_MIN_COMB_TIME  0.015f
#define  TONIC_REVERB_MAX_COMB_TIME  0.035f
#define  TONIC_REVERB_STEREO_SPREAD 0.001f

// Number of allpass filters per channel
#define  TONIC_REVERB_N_ALLPASS 4
#define  TONIC_REVERB_ALLPASS_COEF 0.5f

namespace Tonic { namespace Tonic_{
  
  
  ImpulseDiffuserAllpass::ImpulseDiffuserAllpass(TonicFloat delay, TonicFloat coef)
  {
    delay_ = delay;
    coef_ = coef;
    delayBack_.initialize(delay, 1);
    delayForward_.initialize(delay, 1);
    delayBack_.setInterpolates(false);
    delayForward_.setInterpolates(false);
  }
  
  ImpulseDiffuserAllpass::ImpulseDiffuserAllpass( const ImpulseDiffuserAllpass & other)
  {
    delay_ = other.delay_;
    coef_ = other.coef_;
    delayBack_.initialize(delay_,1);
    delayForward_.initialize(delay_,1);
    delayBack_.setInterpolates(false);
    delayForward_.setInterpolates(false);
  }
  
  // ==============
  
  // Changing these will change the character of the late-stage reverb.
  static const TonicFloat combTimeScales_[TONIC_REVERB_N_COMBS] = {1.17, 1.12, 1.02, 0.97, 0.95, 0.88, 0.84, 0.82};
  static const TonicFloat allpassTimes_[TONIC_REVERB_N_ALLPASS] = {0.0051, 0.010, 0.012, 0.00833};
  
  Reverb_::Reverb_(){
    
    setIsStereoOutput(true);
    
    // Default to 50% wet
    setDryLevelGen(FixedValue(0.5f));
    setWetLevelGen(FixedValue(0.5f));
    
    workspaceFrames_[0].resize(kSynthesisBlockSize, 1, 0);
    workspaceFrames_[1].resize(kSynthesisBlockSize, 1, 0);
    preOutputFrames_[0].resize(kSynthesisBlockSize, 1, 0);
    preOutputFrames_[1].resize(kSynthesisBlockSize, 1, 0);
    
    preDelayLine_.initialize(0.1f, 1);
    reflectDelayLine_.initialize(0.1f, 1);
    
    inputLPF_.setIsStereoInput(false);
    inputLPF_.setIsStereoInput(false);
    inputLPF_.Q(0.707);
    inputHPF_.Q(0.707);
  
    preDelayTimeCtrlGen_ = ControlValue(0.01f);
    inputFiltBypasCtrlGen_ = ControlValue(false);
    densityCtrlGen_ = ControlValue(0.5);
    roomShapeCtrlGen_ = ControlValue(0.5);
    roomSizeCtrlGen_ = ControlValue(0.5);
    decayTimeCtrlGen_ = ControlValue(1.0f);
    stereoWidthCtrlGen_ = ControlValue(0.5f);
    
    setInputLPFCutoffCtrlGen(ControlValue(10000.0f));
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
    
    for (unsigned int i=0; i<TONIC_REVERB_N_ALLPASS; i++){
      allpassFilters_[TONIC_LEFT].push_back(ImpulseDiffuserAllpass(allpassTimes_[i], TONIC_REVERB_ALLPASS_COEF));
      allpassFilters_[TONIC_RIGHT].push_back(ImpulseDiffuserAllpass(allpassTimes_[i] + TONIC_REVERB_STEREO_SPREAD, TONIC_REVERB_ALLPASS_COEF));
    }
    
    setDecayLPFCtrlGen(ControlValue(12000.f));
    setDecayHPFCtrlGen(ControlValue(60.f));
    
  }
  
  void Reverb_:: updateDelayTimes(const SynthesisContext_ & context)
  {
    ControlGeneratorOutput densityOutput = densityCtrlGen_.tick(context);
    ControlGeneratorOutput shapeOutput = roomShapeCtrlGen_.tick(context);
    ControlGeneratorOutput sizeOutput = roomSizeCtrlGen_.tick(context);
    ControlGeneratorOutput decayOutput = decayTimeCtrlGen_.tick(context);
    
    if (densityOutput.triggered ||
        shapeOutput.triggered ||
        sizeOutput.triggered)
    {
      
      reflectTapTimes_.clear();
      reflectTapScale_.clear();
      
      // compute base round-trip times from listener to wall, based on shape and size values
      TonicFloat shape = clamp(shapeOutput.value, 0.f, 1.f);
      TonicFloat size = clamp(sizeOutput.value, 0.f, 1.f);

      TonicFloat wDist1 = map(size, 0.f, 1.f, TONIC_REVERB_MIN_WALL_DIST, TONIC_REVERB_MAX_WALL_DIST, true);
      TonicFloat wDist2 = map(size * (1.1f - shape), 0.f, 1.f, TONIC_REVERB_MIN_WALL_DIST, TONIC_REVERB_MAX_WALL_DIST, true);

      unsigned int nTaps = (unsigned int)map(densityOutput.value, 0.f, 1.f, TONIC_REVERB_MIN_TAPS, TONIC_REVERB_MAX_TAPS, true);
      TonicFloat tapScale = 1.0f/max(2.f, sqrtf(nTaps));
      for (unsigned int i=0; i<nTaps; i++){
        
        TonicFloat dist = (i % 2 == 0 ? wDist1 : wDist2) * (1.0f + randomFloat(-TONIC_REVERB_FUDGE_AMT, TONIC_REVERB_FUDGE_AMT));
        
        reflectTapTimes_.push_back( dist/TONIC_REVERB_SOS );
        reflectTapScale_.push_back( dBToLin(dist * TONIC_REVERB_AIRDECAY)*tapScale );
      }
      
    }
    
    // if decay or room size have changed, need to update comb times/scales
    if (sizeOutput.triggered ||
        decayOutput.triggered)
    {
      
      TonicFloat decayTime = decayOutput.value;
      TonicFloat baseCombDelayTime = map(sizeOutput.value, 0.f, 1.f, TONIC_REVERB_MIN_COMB_TIME, TONIC_REVERB_MAX_COMB_TIME, true);
      
      for (unsigned int i=0; i<TONIC_REVERB_N_COMBS; i++){
        
        TonicFloat scaledDelayTime = combTimeScales_[i % TONIC_REVERB_N_COMBS] * baseCombDelayTime;
        combFilterDelayTimes_[TONIC_LEFT][i].value(scaledDelayTime);
        combFilterDelayTimes_[TONIC_RIGHT][i].value(scaledDelayTime+TONIC_REVERB_STEREO_SPREAD);
        combFilterScaleFactors_[TONIC_LEFT][i].value( powf(10.f, (-3.0f * scaledDelayTime / decayTime)) );
        combFilterScaleFactors_[TONIC_RIGHT][i].value( powf(10.f, (-3.0f * (scaledDelayTime+TONIC_REVERB_STEREO_SPREAD) / decayTime)) );

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
