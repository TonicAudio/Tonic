//
//  DelayTestSynth.cpp
//  TonicDemo
//
//  Created by Nick Donaldson on 3/10/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_DelayTestSynth_h
#define TonicDemo_DelayTestSynth_h

#include "Tonic.h"
#include "RectWave.h"
#include "ADSR.h"
#include "Filters.h"
#include "MonoDelay.h"
#include "ControlMetro.h"
#include "ControlPulse.h"

using namespace Tonic;

class DelayTestSynth : public Synth {
  
public:
  
  DelayTestSynth(){
    
    ControlMetro metro = ControlMetro().bpm(60);
    ControlPulse pulseGate = ControlPulse().in(metro).length(0.1f);
    
    ADSR aEnv = ADSR().attack(0.001f).decay(0.0f).sustain(1.0f).release(0.02f).trigger(pulseGate);
    ADSR fEnv = ADSR().attack(0.01f).decay(0.1f).sustain(0.0f).release(0.01f).trigger(pulseGate);
    
    RectWave osc = RectWave().freq(110.0f).pwm(0.5f);
    LPF12 filt = LPF12().cutoff(60.0f * (1.0f + fEnv * 100.0f)).Q(2.0f);
    
    MonoDelay delay = MonoDelay(0.5f, 1.0f)
      .delayTime( addParameter("delayTime", 0.5f, 0.01f, 1.0f).ramped(0.5f) )
      .feedback( addParameter("feedback", 0.0f, 0.0f, 0.8f).ramped() )
      .mix( 0.35 );
    
    outputGen = (((osc >> filt) * aEnv * 0.5) >> delay);
  }
  
};

registerSynth(DelayTestSynth);

#endif
