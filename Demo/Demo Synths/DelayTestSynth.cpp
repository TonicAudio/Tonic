//
//  DelayTestSynth.cpp
//  TonicDemo
//
//  Created by Nick Donaldson on 3/10/13.

//

#ifndef TonicDemo_DelayTestSynth_h
#define TonicDemo_DelayTestSynth_h

#include "Tonic.h"

using namespace Tonic;

class DelayTestSynth : public Synth {
  
public:
  
  DelayTestSynth(){
    
    ControlValue tempo = addParameter("tempo", "Tempo", 120.f, 60.f, 300.f);
    ControlValue delayTime = addParameter("delayTime", "Delay Time", 0.12f, 0.001f, 1.0f, true); // logarithmic
    ControlValue feedBack = addParameter("feedback", "Delay Feedback", 0.4f, 0.0f, 0.95f);
    ControlValue delayMix = addParameter("delayMix", "Delay Dry/Wet", 0.3f, 0.0f, 1.0f);
    ControlValue decay = addParameter("decayTime", "Env Decay Time", 0.08f, 0.05f, 0.25f, true); // logarithmic
    ControlValue volume = addParameter("volume", "Volume (dbFS)", -12.0f, -60.0f, 0.f);
    
    ControlMetro metro = ControlMetro().bpm(tempo * 4);
    
    ADSR aEnv = ADSR().attack(0.005f).decay(decay).sustain(0.0f).release(0.01f).trigger(metro).doesSustain(false).exponential(true);
    ADSR fEnv = ADSR().attack(0.005f).decay(decay).sustain(0.0f).release(0.01f).trigger(metro).doesSustain(false).exponential(true);
    
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
      .delayTime( delayTime.smoothed(0.5f) )
      .feedback( feedBack.smoothed() )
      .mix( delayMix );
    
    outputGen = (osc >> filt >> delay) * ControlDbToLinear().in(volume).smoothed();
  }
  
};

registerSynth(DelayTestSynth);

#endif
