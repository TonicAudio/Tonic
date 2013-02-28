//
//  SineAMSynth,h
//  Tonic
//
//  Created by Nick Donaldson on 02/07/2013
//
//

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
          addParameter("modFreq", 10, 0, 100).value.ramped()
        ) +
        addParameter("carrierAmt", 1, 0, 1).value.ramped()
      ) *
      SineWave().freq(
        addParameter("carrierFreq", 400, 100, 800).value.ramped(1000)
      )
    ) * 0.5f;
    
  };
  
};

registerSynth(SineAMSynth);

#endif
