//
//  DelayTestSynth.cpp
//  TonicDemo
//
//  Created by Nick Donaldson on 3/10/13.

//

#ifndef TonicDemo_DelayTestSynth_h
#define TonicDemo_DelayTestSynth_h

#include "Tonic.h"
#include "RectWave.h"
#include "ADSR.h"
#include "Filters.h"
#include "MonoToStereoPanner.h"
#include "BasicDelay.h"
#include "ControlMetro.h"
#include "ControlPulse.h"
#include "ControlStepper.h"
#include "ControlMidiToFreq.h"

using namespace Tonic;

class DelayTestSynth : public Synth {
  
public:
  
  DelayTestSynth(){
    
    ControlMetro metro = ControlMetro().bpm(120 * 4);
    ControlPulse pulseGate = ControlPulse().in(metro).length(0.05f);
    
    ADSR aEnv = ADSR().attack(0.001f).decay(0.05f).sustain(0.0f).release(0.02f).trigger(pulseGate);
    ADSR fEnv = ADSR().attack(0.001f).decay(0.1f).sustain(0.0f).release(0.01f).trigger(pulseGate);
    
    ControlStepper step = ControlStepper().start(48).end(84).step(5).trigger(metro);
    ControlMidiToFreq freq = ControlMidiToFreq().in(step);
    
    Generator osc = (
                      (
                        (
                          RectWave().freq( freq.ramped(0.02) * 0.99 ).pwm(0.5f) * aEnv * 0.5
                        )
                        >> MonoToStereoPanner().pan(-1)
                      )
                      +
                      (
                        (
                          RectWave().freq(freq.ramped(0.02) * 1.01).pwm(0.5f) * aEnv * 0.5
                        )
                        >> MonoToStereoPanner().pan(1)
                      )
                    );
    
    LPF12 filt = LPF12().cutoff(400.0f * (1.0f + fEnv*9.0f)).Q(1.5f);
    
    BasicDelay delay = BasicDelay(0.5f, 1.0f)
      .delayTime( addParameter("delayTime", 0.5f, 0.01f, 1.0f).ramped(0.5f) )
      .feedback( addParameter("feedback", 0.0f, 0.0f, 0.8f).ramped() )
      .mix( dBToLin(-10) );
    
    outputGen = ((osc >> filt) >> delay) * 0.8;
  }
  
};

registerSynth(DelayTestSynth);

#endif
