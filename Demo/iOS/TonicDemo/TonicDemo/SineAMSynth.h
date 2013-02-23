//
//  SineAMSynth,h
//  Tonic
//
//  Created by Nick Donaldson on 02/07/2013
//
//

#ifndef _013_1_23_melody_TestSynth_h
#define _013_1_23_melody_TestSynth_h

#include "Tonic.h"
#include "Synth.h"
#include "SineWave.h"

using namespace Tonic;

class SineAMSynth : public Synth{

public:

  SineAMSynth() {
    
    outputGen =
    (
      ( SineWave().freq(
          RampedValue(10).target( registerMessage("modFreq", 10) )
        ) +
        RampedValue(1).target( registerMessage("carrierAmt", 1) )
      ) *
      SineWave().freq(
        RampedValue(400, 1000).target( registerMessage("carrierFreq", 400) )
      )
    ) * 0.5f;
    
  };
  
private:
  
  static SourceRegister<SineAMSynth> reg;
  
};

SourceRegister<SineAMSynth> SineAMSynth::reg("SineAMSynth");

#endif
