//
//  ADSRTestSynth.h
//  TonicDemo
//
//  Created by Morgan Packard on 2/28/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

/*
Because of the fancy registerSynth macro, and because this synth has the same interface
as Synth it's not necessary to create a header file. You can create an instance of this synth using 
SynthFactory::createInstance(SYNTH_NAME), where SYNTH_NAME is the name supplied to registerSynth.
*/

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
