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
    
    ControlValue preDelay = addParameter("preDelay", "Pre-delay", 0.001f, 0.001f, 0.2f, true);
    ControlValue inputLPF = addParameter("inputLPF", "Input LPF cutoff (Hz)", 20.f, 20.f, 400.f);
    ControlValue inputHPF = addParameter("inputHPF", "Input HPF cutoff (Hz)", 12000.0f, 4000.f, 18000.f);
    ControlValue density = addParameter("density", "Density", 0.5f, 0.f, 1.f);
    ControlValue shape = addParameter("shape", "Shape", 0.5f, 0.f, 1.f);
    ControlValue size = addParameter("size", "Size", 0.5f, 0.f, 1.f);
    ControlValue dry = addParameter("dry", "Dry Level (dbFS)", -12.f, -60.f, 0.f);
    ControlValue wet = addParameter("wet", "Wet Level (dbFS)", -60.f, -60.f, 0.f);
    
    float bpm = 30.f;
    
    ControlMetro beat = ControlMetro().bpm(bpm);
    ControlDelay offbeat = ControlDelay(1.2f).in(beat).delayTime(30.0f/bpm);
    
    Generator click = Noise() * ADSR(0.0001f,0.025f,0,0.01f).doesSustain(false).exponential(true).trigger(beat);
    
    Generator tone = RectWave().pwm(0.5f).freq(Tonic::mtof(60)) * ADSR(0.001f, 0.08f, 0, 0.01f).doesSustain(false).exponential(true).trigger(offbeat);
    
    Reverb reverb = Reverb()
      .preDelayTime(preDelay)
      .inputLPFCutoff(inputHPF)
      .inputHPFCutoff(inputHPF)
      .density(density)
      .roomShape(shape)
      .roomSize(size)
      .dryLevel(ControlDbToLinear().in(dry))
      .wetLevel(ControlDbToLinear().in(wet));
    
    outputGen = ((click + tone) >> reverb) * 0.8f;
  }
  
};

registerSynth(ReverbTestSynth);