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
    
    ControlParameter dry = addParameter("dry",-6.f).displayName("Dry Level (dbFS)").min(-60.f).max(0.f);
    ControlParameter wet = addParameter("wet",-20.f).displayName("Wet Level (dbFS)").min( -60.f).max(0.f);
    ControlParameter time = addParameter("decayTime", 1.0f).displayName("Decay Time (s)").min(0.1f).max(10.f);
    ControlParameter lowDecay = addParameter("lowDecay", 16000.0f).displayName("Decay Lowpass Cutoff (Hz)").min(4000.0f).max(20000.0f);
    ControlParameter hiDecay = addParameter("hiDecay",20.0f).displayName("Decay Highpass Cutoff (Hz)").min(20.f).max(250.f);
    ControlParameter preDelay = addParameter("preDelay", 0.001f).displayName("Pre-delay").min(0.001f).max(0.05f);
    ControlParameter inputLPF = addParameter("inputLPF",18000.f).displayName("Input LPF cutoff (Hz)").min(4000.0f).max(20000.0f);
    ControlParameter inputHPF = addParameter("inputHPF",20.f).displayName("Input HPF cutoff (Hz)").min(20.f).max(250.f);
    ControlParameter density = addParameter("density",0.5f).displayName("Density");
    ControlParameter shape = addParameter("shape",0.5f).displayName("Shape");
    ControlParameter size = addParameter("size",0.5f).displayName("Room Size");
    ControlParameter stereo = addParameter("stereo",0.5f).displayName("Stereo Width");

    float bpm = 30.f;
    
    ControlMetro beat = ControlMetro().bpm(bpm);
    ControlDelay offbeat = ControlDelay(1.2f).input(beat).delayTime(30.0f/bpm);
    
    Generator click = Noise() * ADSR(0.0001f,0.025f,0,0.01f).doesSustain(false).exponential(true).trigger(beat);
    
    Generator tone = RectWave().pwm(0.5f).freq(Tonic::mtof(60)) * ADSR(0.001f, 0.08f, 0, 0.01f).doesSustain(false).exponential(true).trigger(offbeat);
    
    Reverb reverb = Reverb()
      .preDelayTime(preDelay)
      .inputLPFCutoff(inputLPF)
      .inputHPFCutoff(inputHPF)
      .decayTime(time)
      .decayLPFCutoff(lowDecay)
      .decayHPFCutoff(hiDecay)
      .stereoWidth(stereo)
      .density(density)
      .roomShape(shape)
      .roomSize(size)
      .dryLevel(ControlDbToLinear().input(dry))
      .wetLevel(ControlDbToLinear().input(wet));
        
    setOutputGen( ((click + tone) >> reverb) * 0.8f );
  }
  
};

TONIC_REGISTER_SYNTH(ReverbTestSynth);