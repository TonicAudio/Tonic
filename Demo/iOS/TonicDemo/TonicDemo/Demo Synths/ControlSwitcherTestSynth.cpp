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
#include "ControlStepper.h"
#include "ControlRandom.h"
#include "Filters.h"
#include "RectWave.h"

using namespace Tonic;

class ControlSwitcherTestSynth : public Synth{

public:
  ControlSwitcherTestSynth(){
  
    ControlMetro metro = ControlMetro().bpm(80 * 4);
    ControlGenerator modeSwitch =  ControlMetro().bpm(4);
    
    const int numSteps = 11;
    ControlCounter step = ControlCounter()
      .trigger(metro)
      .end(
      ControlSwitcher()
      .addInput(numSteps)
      .addInput( ControlRandom().min(2).max(6).trigger(modeSwitch) )
      .inputIndex(
        ControlCounter()
        .end(2)
        .trigger(modeSwitch)
         -1)
       -1);
    
    ControlGenerator clickVol = addParameter("clickVol");
    ADSR clickEnv =  ADSR(0, 0.01, 0, 0).trigger(metro);
    Generator clickIntensity = (1 + clickVol).ramped();
    Generator click = SineWave().freq(300 + 1000 * clickEnv ) * clickEnv * 0.03 ;
    
    ControlSwitcher fmAmount = ControlSwitcher().inputIndex(step);
    ControlSwitcher sustain = ControlSwitcher().inputIndex(step);
    ControlSwitcher decay = ControlSwitcher().inputIndex(step);
    ControlSwitcher spreadSeq = ControlSwitcher().inputIndex(step);
    
    for(int i = 0; i < numSteps; i++){
      fmAmount.addInput(randomFloat(1, 10));
      sustain.addInput(randomFloat(0, 0.7));
      decay.addInput(randomFloat(0.03, 0.1));
      spreadSeq.addInput(randomFloat(0, 0.5));
    }
    
    Generator freq = ControlValue(50).ramped();
    Generator tremelo =  1 + ( SineWave().freq(15) *  ADSR(0, 0.5, 0,0).trigger(modeSwitch) );
    Generator bassEnv = ADSR(0.001, 0.1 ,0,0).decay(decay).legato(true).sustain(sustain * sustain).trigger(metro);
    ControlGenerator spread = ControlRandom().min(0).max(0.5).trigger(modeSwitch);// * spreadSeq;
    Generator bass =
    
    (
      RectWave()
      .pwm(0.5 + 0.02 * bassEnv)
      .freq(
        freq
        + freq
          * SineWave()
            .freq(freq * 2)
          * (
            fmAmount
            * 0.5
            + addParameter("addtlFM")
            ).ramped()
      )
      >> MonoToStereoPanner().pan(-1 * spread)
     )
    
      +
    
    (
      RectWave()
      .pwm(0.5 + 0.02 * bassEnv)
      .freq(
        freq * 1.02
        + freq
          * SineWave()
            .freq(freq * 2)
          * (
            fmAmount
            * 0.5
            + addParameter("addtlFM")
            ).ramped()
      )
      >> MonoToStereoPanner().pan(spread)
     )
    
    
      >> HPF12().cutoff(addParameter("hpf") * 1000)
      >> LPF12().cutoff(5500) ;
    Generator bassWithAmp = bass * bassEnv * tremelo;
    outputGen = bassWithAmp  + click;
  }
};

registerSynth(ControlSwitcherTestSynth)

#endif
