//
//  SquareWave.h
//  TonicLib
//
//  Created by Nick Donaldson on 5/17/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#ifndef TonicLib_SquareWave_h
#define TonicLib_SquareWave_h

#include "RectWave.h"

namespace Tonic
{
  
  // The PWM of a RectWave_ or RectWaveBL_ is 0.5 by default (square)
  
  //! Quck-and-dirty square wave
  class SquareWave : public TemplatedGenerator<Tonic_::RectWave_>{
    
  public:

    TONIC_MAKE_GEN_SETTERS(SquareWave, freq, setFrequencyGenerator);

  };
  
  //! Bandlimited square wave
  class SquareWaveBL : public TemplatedGenerator<Tonic_::RectWaveBL_>{
    
  public:
    
    TONIC_MAKE_GEN_SETTERS(SquareWaveBL, freq, setFreqGen);
    
  };
}

#endif
