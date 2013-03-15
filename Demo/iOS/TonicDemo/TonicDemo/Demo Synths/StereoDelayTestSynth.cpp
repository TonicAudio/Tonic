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
#include "Filters.h"
#include "SineWave.h"

using namespace Tonic;

class StereoDelayTestSynth : public Synth {
  
public:
  ControlMetro metro = ControlMetro().bpm(100);
  ControlGenerator freq = ControlRandom().trigger(metro).min(0).max(1);
  StereoDelayTestSynth(){
  
  
  
  
    SineWave filterSine = SineWave().freq(0.01);
    ADSR env = ADSR()
      .attack(0.01)
      .decay( addParameter("decay") )
      .sustain(0)
      .release(0)
      .doesSustain(false)
      .trigger(metro);
    outputGen = StereoDelay().input(
      RectWave().freq(
        freq * addParameter("frequencyRandomAmount") + 100
        + addParameter("freq")
      )
      * env
      * SineWave().freq(50)
    )
    .delayTimeLeft(0.5)
    .delayTimeRight(0.55);
    outputGen = (outputGen * 0.5) >> LPF24().cutoff((filterSine + 1) * 200 + 400);
  }
  
  
  
  
  
};

registerSynth(StereoDelayTestSynth)

#endif
