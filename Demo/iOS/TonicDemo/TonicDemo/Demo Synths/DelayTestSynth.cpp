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
#include "ControlMidiToFreq.h"
#include "ControlRandom.h"
#include "ControlSnapToScale.h"

using namespace Tonic;

class DelayTestSynth : public Synth {
  
public:
  
  DelayTestSynth(){
    
    ControlMetro metro = ControlMetro().bpm(120 * 4);
    ControlPulse pulseGate = ControlPulse().in(metro).length(0.05f);
    
    ADSR aEnv = ADSR().attack(0.005f).decay(0.08f).sustain(0.0f).release(0.01f).trigger(pulseGate).doesSustain(false).exponential(true);
    ADSR fEnv = ADSR().attack(0.005f).decay(0.08f).sustain(0.0f).release(0.01f).trigger(pulseGate).doesSustain(false).exponential(true);
    
    float scalenums[5] = {0,3,5,7,10};
    std::vector<float> scale(scalenums, scalenums + 5);
    
    ControlRandom rand = ControlRandom().min(0).max(36).trigger(metro);
    ControlSnapToScale snap = ControlSnapToScale().setScale(scale).in(rand);
    ControlMidiToFreq freq = ControlMidiToFreq().in(48 + snap);
    
    Generator osc = (
                      (
                        (
                          RectWave().freq( freq * 0.99 ).pwm(0.5f) * aEnv * 0.5
                        )
                        >> MonoToStereoPanner().pan(-0.5)
                      )
                      +
                      (
                        (
                          RectWave().freq(freq * 1.01).pwm(0.5f) * aEnv * 0.5
                        )
                        >> MonoToStereoPanner().pan(0.5)
                      )
                    );
    
    LPF12 filt = LPF12().cutoff(400.0f * (1.0f + fEnv*9.0f)).Q(1.1f);
    
    BasicDelay delay = BasicDelay(0.5f, 1.0f)
      .delayTime( addParameter("delayTime", 0.4f, 0.01f, 1.0f).smoothed(0.5f) )
      .feedback( addParameter("feedback", 0.0f, 0.0f, 0.8f).smoothed() )
      .mix( dBToLin(-10) );
    
    outputGen = (osc >> filt >> delay) * 0.8;
  }
  
};

registerSynth(DelayTestSynth);

#endif
