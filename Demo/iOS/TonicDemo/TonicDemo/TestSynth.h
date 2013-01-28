//
//  TestSynth.h
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/23/13.
//
//

#ifndef _013_1_23_melody_TestSynth_h
#define _013_1_23_melody_TestSynth_h

#include "Tonic.h"
#include "Synth.h"
#include "SineWave.h"
#include "MonoToStereo.h"
#include "Multiplier.h"
#include "FixedValue.h"
#include "Adder.h"

using namespace Tonic;

class TestSynth : public Synth{

public:

  TestSynth(){
    
    outputGen =
    (
      (
        SineWave().freq(
          300 + 50 * SineWave().freq(2)
        )
        * SineWave().freq(1000)
      )
      +
       SineWave().freq(1000)
    ) * 0.5;
    
  };


};

#endif
