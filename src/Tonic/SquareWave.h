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

namespace Tonic {
  
  //! Quck-and-dirty square wave
  class SquareWave : public TemplatedGenerator<Tonic_::RectWave_>{
    
  public:
    
    SquareWave(){
      gen()->setPwmGenerator(FixedValue(0.5f));
    }
    
    createGeneratorSetters(SquareWave, freq, setFrequencyGenerator);
    
  };

}

#endif
