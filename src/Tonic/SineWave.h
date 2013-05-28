//
//  SineWave.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/8/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__SineWave__
#define __Tonic__SineWave__

#include "TableLookupOsc.h"

namespace Tonic {
  
  class SineWave : public TemplatedGenerator<Tonic_::TableLookupOsc_>{
    
  public:
    
    SineWave();
    
    createGeneratorSetters(SineWave, freq, setFrequency)

  };
}

#endif /* defined(__Tonic__SineWave__) */


