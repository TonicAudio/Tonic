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
    
    ControlParameter tempo = addParameter("tempo", 120.f).displayName("Tempo").min(60.f).max(300.f);
    ControlParameter delayTime = addParameter("delayTime", 0.12f).displayName("Delay Time").min( 0.001f).max(1.0f).logarithmic(true); // logarithmic
    ControlParameter feedBack = addParameter("feedback", 0.4f).displayName("Delay Feedback").min(0.0f).max(0.95f);
    ControlParameter delayMix = addParameter("delayMix", 0.3f).displayName("Delay Dry/Wet").min(0.0f).max(1.0f);
    ControlParameter decay = addParameter("decayTime", 0.08f).displayName("Env Decay Time").min(0.05f).max(0.25f).logarithmic(true); // logarithmic
    ControlParameter volume = addParameter("volume", -6.f).displayName("Volume (dbFS)").min(-60.0f).max(0.f);
    
    ControlMetro metro = ControlMetro().bpm(tempo * 4);
    
    ADSR aEnv = ADSR().attack(0.005f).decay(decay).sustain(0.0f).release(0.01f).trigger(metro).doesSustain(false).exponential(true);
    ADSR fEnv = ADSR().attack(0.005f).decay(decay).sustain(0.0f).release(0.01f).trigger(metro).doesSustain(false).exponential(true);
    
    float scalenums[5] = {0,3,5,7,10};
    std::vector<float> scale(scalenums, scalenums + 5);
    
    ControlRandom rand = ControlRandom().min(0).max(36).trigger(metro);
    ControlSnapToScale snap = ControlSnapToScale().setScale(scale).input(rand);
    ControlMidiToFreq freq = ControlMidiToFreq().input(48 + snap);
    
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
    
    Generator smoothMix = delayMix.smoothed();
    
    BasicDelay delay = BasicDelay(0.5f, 1.0f)
      .delayTime( delayTime.smoothed(0.5f) )
      .feedback( feedBack.smoothed() )
      .dryLevel( 1.0f - smoothMix )
      .wetLevel( smoothMix );
    
    setOutputGen((osc >> filt >> delay) * ControlDbToLinear().input(volume).smoothed());

  }
  
};

TONIC_REGISTER_SYNTH(DelayTestSynth);

#endif
