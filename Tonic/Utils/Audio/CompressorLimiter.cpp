//
//  CompressorLimiter.cpp
//  Tonic
//
//  Created by Nick Donaldson on 3/24/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "CompressorLimiter.h"

namespace Tonic { namespace Tonic_{
  
  Compressor_::Compressor_() : isLimiter_(false), gainEnvValue_(0), ampEnvValue_(0) {
    ampInputFrames_.resize(kSynthesisBlockSize, 1, 0);
    lookaheadDelayLine_.initialize(0.001, 0.01, 2);
    lookaheadDelayLine_.setInterpolates(false); // No real need to interpolate here for lookahead
  }
  
  Compressor_::~Compressor_(){
    
  }
  
  void Compressor_::setIsStereo(bool isStereo){
    setIsStereoInput(isStereo);
    setIsStereoOutput(isStereo);
    ampInputFrames_.resize(kSynthesisBlockSize, isStereo ? 2 : 1, 0);
  }
  
} // Namespace Tonic_

#pragma mark - Limiter
  
  Limiter::Limiter(){
    gen()->setIsLimiter(true);
    gen()->setAttack(ControlValue(0.0001));
    gen()->setLookahead(ControlValue(0.003));
    gen()->setRelease(ControlValue(0.080));
    gen()->setThreshold(ControlValue(dBToLin(-0.1)));
    gen()->setRatio(ControlValue(std::numeric_limits<float>::max()));
  }
  
  
} // Namespace Tonic
