//
//  ControlInputTestSynth.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 4/15/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_ControlInputTestSynth_cpp
#define TonicDemo_ControlInputTestSynth_cpp

#include "Synth.h"
#include "RectWave.h"
#include "Filters.h"
#include "SineWave.h"
#include "ControlXYSpeed.h"
#include "StereoDelay.h"
#include "ControlMetro.h"
#include "ADSR.h"
#include "ControlSwitcher.h"
#include "ControlCounter.h"

using namespace Tonic;

class ControlSwitcherTestSynth : public Synth{
public:
  ControlSwitcherTestSynth(){
  
    ControlMetro metro = ControlMetro().bpm(80 * 4);
    
    const int numSteps = 11;
    ControlCounter step = ControlCounter()
      .trigger(metro)
      .end(numSteps - 1);
    
    ControlSwitcher fmAmount = ControlSwitcher().inputIndex(step);
    ControlSwitcher sustain = ControlSwitcher().inputIndex(step);
    
    for(int i = 0; i < numSteps; i++){
      
      fmAmount.addInput(randomFloat(1, 10) + randomFloat(-5, 5) * addParameter("x"));
      sustain.addInput(randomFloat(0, 0.7) + randomFloat(-0.5, 0.3) * addParameter("y"));
    }
    
    ControlGenerator freq = ControlValue(50);
    outputGen = SineWave().freq(freq.ramped() + freq.ramped() * SineWave().freq(freq * 2) * fmAmount.ramped());
    outputGen = outputGen * ADSR(0.001, 0.1,0,0).legato(true).sustain(sustain).trigger(metro);
  }
};

registerSynth(ControlSwitcherTestSynth)

#endif
