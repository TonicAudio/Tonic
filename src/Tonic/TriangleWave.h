//
//  TriangleWave.h
//  Tonic
//
//  Created by Nick Donaldson on 5/17/13.
//
//
// See LICENSE.txt for license and usage information.
//

#ifndef TONIC_TRIANGLEWAVE_H
#define TONIC_TRIANGLEWAVE_H

#include "SawtoothWave.h"

namespace Tonic {
  
  //! Quick-and-dirty triangle wave.
  class TriangleWave : public TemplatedGenerator<Tonic_::AngularWave_>
  {
    
  public:
    
    TriangleWave(){
      gen()->setSlopeGenerator(FixedValue(0.5f));
    }
    
    TONIC_MAKE_GEN_SETTERS(TriangleWave, freq, setFrequencyGenerator);
    
    //! Set from 0-1 to change slope. At 0, it's a falling saw, at 1, it's a rising saw (defaults to 0.5, triangle)
    TONIC_MAKE_GEN_SETTERS(TriangleWave, slope, setSlopeGenerator);
    
  };
  
}

#endif
