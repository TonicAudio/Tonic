//
//  StereoDelayTestSynth.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 3/13/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_StereoDelayTestSynth_cpp
#define TonicDemo_StereoDelayTestSynth_cpp

#include "Synth.h"
#include "StereoDelay.h"
#include "ControlMetro.h"
#include "RectWave.h"
#include "ADSR.h"
#include "ControlRandom.h"
#include "SineWave.h"

using namespace Tonic;

class StereoDelayTestSynth : public Synth {
  
public:
  ControlMetro metro = ControlMetro().bpm(100);
  ControlGenerator freq = ControlValue(300);// ControlRandom().trigger(metro).min(100).max(1000);
  StereoDelayTestSynth(){
    ADSR env = ADSR()
      .attack(0.01)
      .decay(0.1)
      .sustain(0)
      .release(0)
      .doesSustain(false)
      .trigger(metro);
    outputGen = StereoDelay().input( RectWave().freq( freq ) * env )
      .delayTimeLeft(0.5 + SineWave().freq(0.1) * 0.2)
      .delayTimeRight(0.5 + SineWave().freq(0.15) * 0.3);
  }
};

registerSynth(StereoDelayTestSynth)

#endif
