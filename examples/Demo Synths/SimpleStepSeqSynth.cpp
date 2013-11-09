//
//  SimpleStepSeqSynth.h
//  TonicDemo
//
//  Created by Morgan Packard on 5/14/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_SimpleStepSeqSynth_h
#define TonicDemo_SimpleStepSeqSynth_h


#include "Tonic.h"

using namespace Tonic;

class SimpleStepSeqSynth : public Synth {
  

public:
  SimpleStepSeqSynth(){
      
#if TONIC_HAS_CPP_11
  
    const int NUM_STEPS = 8;
  
    ControlGenerator metro = ControlMetro().bpm(4 * addParameter("tempo",100).min(50).max(300));
    
    ControlGenerator step = ControlStepper().end(NUM_STEPS).trigger(metro);
    ControlGenerator transpose = addParameter("transpose", 0).min(-6).max(6);
    ControlSwitcher pitches = ControlSwitcher().inputIndex(step);
    ControlSwitcher cutoffs = ControlSwitcher().inputIndex(step);
    
    for(int i = 0; i < NUM_STEPS; i++){
      float initialValue =  randomFloat(10, 80);
      pitches.addInput(addParameter("step" + to_string(i) + "Pitch", initialValue).min(10).max(80));
      cutoffs.addInput(addParameter("step" + to_string(i) + "Cutoff", 500).min(30).max(1500));
    }
  
    ControlGenerator midiNote = transpose + (pitches >> ControlSnapToScale().setScale({0,2,3,5,7,10}));
  
    Generator outputGen = SquareWave().freq( midiNote >> ControlMidiToFreq() ) * ADSR(0.01, 0.1, 0,0).trigger(metro) * 0.1;
    outputGen = (outputGen >> LPF24().cutoff(cutoffs).Q(0.1));
    //outputGen = outputGen >> Reverb().wetLevel(0.1).dryLevel(1).decayTime(1);

    setOutputGen(outputGen);
    
#else
      
#pragma message("C++11 Support not available - SimpleStepSeqSynth disabled")
      
#endif
      
  }
    
};

TONIC_REGISTER_SYNTH(SimpleStepSeqSynth);


#endif
