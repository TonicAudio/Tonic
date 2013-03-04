//
//  ADSRTestSynth.h
//  TonicDemo
//
//  Created by Morgan Packard on 2/28/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_ADSRTestSynth_h
#define TonicDemo_ADSRTestSynth_h

#include "Synth.h"
#include "Noise.h"
#include "ADSR.h"

using namespace Tonic;

class ADSRTestSynth : public Synth {
  
public:
  ADSRTestSynth(){

    outputGen = ADSR()
      .trigger( addParameter("trigger") )
      .attack(0.1)
      .decay(0.5)
      .sustain(0.5)
      .release(0.5)
      .doesSustain(true)
       * Noise() * 0.1;
  }
  
};

registerSynth(ADSRTestSynth);
#endif
