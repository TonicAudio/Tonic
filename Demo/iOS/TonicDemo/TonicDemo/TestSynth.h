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

using namespace Tonic;

class TestSynth : public Synth{

public:

  TestSynth(){

    SineWave* sineWave = new SineWave();
    sineWave->setFrequency(200);
    MonoToStereo* panner = new MonoToStereo();
    panner->in(sineWave);
    outputGen = panner;
  };


};

#endif
