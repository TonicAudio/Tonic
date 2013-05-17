//
//  TriangleWave.h
//  TonicLib
//
//  Created by Nick Donaldson on 5/17/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#ifndef TonicLib_TriangleWave_h
#define TonicLib_TriangleWave_h

#include "SawtoothWave.h"

namespace Tonic {
  
  //! Quick-and-dirty triangle wave.
  class TriangleWave : public TemplatedGenerator<Tonic_::AngularWave_>
  {
    
  public:
    
    TriangleWave(){
      gen()->setSlopeGenerator(FixedValue(0.5f));
    }
    
    createGeneratorSetters(TriangleWave, freq, setFrequencyGenerator);
    
    //! Set from 0-1 to change slope. At 0, it's a falling saw, at 1, it's a rising saw (defaults to 0.5, triangle)
    createGeneratorSetters(TriangleWave, slope, setSlopeGenerator);
    
  };
  
}

#endif
