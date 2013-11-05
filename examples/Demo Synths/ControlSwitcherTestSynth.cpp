//
//  ControlInputTestSynth.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 4/15/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_ControlInputTestSynth_cpp
#define TonicDemo_ControlInputTestSynth_cpp

#include "Tonic.h"
#include <functional>

using namespace Tonic;

class ControlSwitcherTestSynth : public Synth{

public:
  ControlSwitcherTestSynth(){
  
#if TONIC_HAS_CPP_11
      
    ControlMetro metro = ControlMetro().bpm(80 * 4);
    ControlGenerator modeSwitch =  ControlMetro().bpm(4);
    
    const int numSteps = 20;
    
    ControlGenerator phraseStart = ControlRandom().min(0).max(5).trigger(modeSwitch);
    
    ControlGenerator phraseLen = ControlRandom().min(3).max(11).trigger(modeSwitch);
    
    ControlStepper step = ControlStepper()
      .start(phraseStart)
      .end(phraseStart + phraseLen - 1)
      .trigger(metro);
    
    ControlGenerator clickVol = addParameter("clickVol");
    ADSR clickEnv =  ADSR(0, 0.01, 0, 0).trigger(metro);
    Generator clickIntensity = (1 + clickVol).smoothed();
    Generator click = SineWave().freq(300 + 1000 * clickEnv ) * clickEnv * 0.03 ;
    
    ControlSwitcher fmAmount = ControlSwitcher().inputIndex(step);
    ControlSwitcher sustain = ControlSwitcher().inputIndex(step);
    ControlSwitcher decay = ControlSwitcher().inputIndex(step);
    ControlSwitcher spreadSeq = ControlSwitcher().inputIndex(step);
    
    for(int i = 0; i < numSteps; i++){
      fmAmount.addInput(randomFloat(0, 10));
      sustain.addInput(randomFloat(0, 0.7));
      decay.addInput(randomFloat(0.03, 0.1));
      spreadSeq.addInput(randomFloat(0, 0.5));
    }
    
    Generator freq = ControlValue(50).smoothed();
    Generator tremelo =  1 + ( SineWave().freq(15) *  ADSR(0, 0.5, 0,0).trigger(modeSwitch) );
    Generator bassEnv = ADSR(0.001, 0.1 ,0,0).decay(decay).legato(true).sustain(sustain * sustain).trigger(metro);
    ControlGenerator spread = ControlRandom().min(0).max(0.5).trigger(modeSwitch);// * spreadSeq;
    ControlGenerator wave = ControlRandom().min(0.4).max(0.9).trigger(modeSwitch);
    
    auto makeBass = [&](Generator bassFreq){
      return RectWave()
      .pwm(wave + 0.04 * bassEnv)
      .freq(
        bassFreq
        + bassFreq
          * SineWave()
            .freq(bassFreq * 1.99)
          * (
            fmAmount
            * 0.7
            + addParameter("addtlFM")
            ).smoothed()
      );
    };
    
    Generator bass =
    (
      makeBass(freq)
      >> MonoToStereoPanner().pan(-1 * spread)
    )
    +
    (
      makeBass(freq * 1.02)
      >> MonoToStereoPanner().pan(1 * spread)
    )
      
     // >> HPF12().cutoff(addParameter("hpf") * 1000)
      >> LPF12().cutoff(5500)
      >> StereoDelay(1.1, 1.2).wetLevel(0.1) ;
    Generator bassWithAmp = bass * bassEnv * tremelo;
    setOutputGen(bassWithAmp  + click);
#else
      
#pragma message("C++11 Support not available - ControlSwitcherTestSynth disabled")
      
#endif
  }
};

TONIC_REGISTER_SYNTH(ControlSwitcherTestSynth)

#endif
