//
//  CompressorLimiter.cpp
//  Tonic
//
//  Created by Nick Donaldson on 3/24/13.

//

#include "CompressorLimiter.h"

namespace Tonic { namespace Tonic_{
  
  Compressor_::Compressor_() : isLimiter_(false), gainEnvValue_(1.f), ampEnvValue_(0) {
    ampInputFrames_.resize(kSynthesisBlockSize, 1, 0);
    lookaheadDelayLine_.initialize(0.01, 2);
    lookaheadDelayLine_.setInterpolates(false); // No real need to interpolate here for lookahead
    makeupGainGen_ = ControlValue(1.f);
  }

  // Default inherited input method sets both audio signal and amplitude signal as input
  // so incoming signal is compressed based on its own amplitude
  void Compressor_::setInput(Generator input){
    setAmplitudeInput(input);
    setAudioInput(input);
  }
  
  void Compressor_::setAudioInput( Generator gen ) {
    input_ = gen;
    setIsStereoInput(gen.isStereoOutput());
    setIsStereoOutput(gen.isStereoOutput());
  }
  
  void Compressor_::setAmplitudeInput( Generator gen ) {
    amplitudeInput_ = gen;
    ampInputFrames_.resize(kSynthesisBlockSize, amplitudeInput_.isStereoOutput() ? 2 : 1, 0);
  }
  
  void Compressor_::setIsStereo(bool isStereo){
    setIsStereoInput(isStereo);
    setIsStereoOutput(isStereo);
    ampInputFrames_.resize(kSynthesisBlockSize, isStereo ? 2 : 1, 0);
  }
  
} // Namespace Tonic_
  
  Compressor::Compressor(float threshold, float ratio, float attack, float release, float lookahead)
  {
    gen()->setThreshold(ControlValue(threshold));
    gen()->setRatio(ControlValue(ratio));
    gen()->setAttack(ControlValue(attack));
    gen()->setRelease(ControlValue(release));
    gen()->setLookahead(ControlValue(lookahead));
  }


#pragma mark - Limiter
  
  Limiter::Limiter(){
    gen()->setIsLimiter(true);
    gen()->setAttack(ControlValue(0.0001));
    gen()->setLookahead(ControlValue(0.003));
    gen()->setRelease(ControlValue(0.080));
    gen()->setThreshold(ControlValue(dBToLin(-0.1)));
    gen()->setRatio(ControlValue(std::numeric_limits<float>::infinity()));
  }
  
  
} // Namespace Tonic
