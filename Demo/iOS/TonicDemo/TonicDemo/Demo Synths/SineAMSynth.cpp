//
//  SineAMSynth,h
//  Tonic
//
//  Created by Nick Donaldson on 02/07/2013
//
//

/*
Because of the fancy registerSynth macro, and because this synth has the same interface
as Synth it's not necessary to create a header file. You can create an instance of this synth using 
SynthFactory::createInstance(SYNTH_NAME), where SYNTH_NAME is the name supplied to registerSynth.
*/

#ifndef _013_1_23_melody_TestSynth_h
#define _013_1_23_melody_TestSynth_h

#include "Synth.h"
#include "SineWave.h"

using namespace Tonic;

class SineAMSynth : public Synth{

public:

  SineAMSynth() {
    
    outputGen =
    (
      ( SineWave().freq(
          addParameter("modFreq", 10, 0, 100).ramped()
        ) +
        addParameter("carrierAmt", 1, 0, 1).ramped()
      ) *
      SineWave().freq(
        addParameter("carrierFreq", 400, 100, 800).ramped(1.0)
      )
    ) * 0.5f;
    
  };
  
};

registerSynth(SineAMSynth);

#endif
