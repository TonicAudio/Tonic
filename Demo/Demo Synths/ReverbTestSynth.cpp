//
//  ReverbTestSynth.cpp
//  TonicDemo
//
//  Created by Nick Donaldson on 5/5/13.
//

#include "Tonic.h"

using namespace Tonic;

class ReverbTestSynth : public Synth {
  
public:
  
  ReverbTestSynth(){
    
    float bpm = 30.f;
    
    ControlMetro beat = ControlMetro().bpm(bpm);
    ControlDelay offbeat = ControlDelay(1.2f).in(beat).delayTime(30.0f/bpm);
    
    Generator click = Noise() * ADSR(0.0001f,0.025f,0,0.01f).doesSustain(false).exponential(true).trigger(beat);
    
    Generator tone = RectWave().pwm(0.5f).freq(Tonic::mtof(60)) * ADSR(0.001f, 0.08f, 0, 0.01f).doesSustain(false).exponential(true).trigger(offbeat);
    
    outputGen = (click + tone) * 0.8f;
  }
  
};

registerSynth(ReverbTestSynth);