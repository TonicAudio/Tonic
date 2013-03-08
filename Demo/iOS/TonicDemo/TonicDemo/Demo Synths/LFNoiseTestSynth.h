//
//  LFNoiseTest.h
//  TonicDemo
//
//  Created by Morgan Packard on 2/26/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_LFNoiseTest_h
#define TonicDemo_LFNoiseTest_h

#include "Tonic.h"
#include "Synth.h"
#include "SineWave.h"
#include "Filters.h"
#include "LFNoise.h"
#include "ControlConditioner.h"

using namespace Tonic;

class LFNoiseTestSynth : public Synth{
public:
  LFNoiseTestSynth(){

    outputGen = SineWave().freq(
      500 + 500 * LFNoise().setFreq(
        addParameter("noiseFreq", 100) )
      )
       * 0.3;
    
  }
};

registerSynth(LFNoiseTestSynth);


#endif
