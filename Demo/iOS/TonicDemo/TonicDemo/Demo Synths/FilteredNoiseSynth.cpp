//
//  FilteredNoise.h
//  TonicDemo
//
//  Created by Morgan Packard on 2/24/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

/*
Because of the fancy registerSynth macro, and because this synth has the same interface
as Synth it's not necessary to create a header file. You can create an instance of this synth using 
SynthFactory::createInstance(SYNTH_NAME), where SYNTH_NAME is the name supplied to registerSynth.
*/

#ifndef TonicDemo_FilteredNoise_h
#define TonicDemo_FilteredNoise_h

#include "Synth.h"
#include "SineWave.h"
#include "Filters.h"
#include "Noise.h"
#include "RampedValue.h"
#include "ControlFloor.h"
#include "ControlMidiToFreq.h"
#include "Noise.h"

using namespace Tonic;

class FilteredNoiseSynth : public Synth{
public:
  FilteredNoiseSynth(){
    vector<float> midiNums;
    midiNums.push_back(-24);
    midiNums.push_back(0);
    midiNums.push_back(3);
    midiNums.push_back(7);
    midiNums.push_back(10);
    
    PinkNoise noise = PinkNoise();
    
    ControlGenerator cutoffCtrl = addParameter("cutoff", 0.5);
    
    Generator q_v = addParameter("Q", 5).ramped();
    
    Adder sumOfFilters;
    
    for(int i = 0; i < midiNums.size(); i++){
      Generator tremelo = (SineWave().freq( randomFloat(0.1, 0.3) ) + 1.5) * 0.3;
      Generator cutoff = ControlMidiToFreq().in( ControlFloor().in( midiNums.at(i) + cutoffCtrl * 100  )).ramped().length(0.01);
      BPF24 filter = BPF24().Q( q_v ).cutoff( cutoff );
      sumOfFilters = sumOfFilters + (noise >> filter) * tremelo;
    }
    
    // add a bit of gain for higher Q
    // Using this to test output limiter as well - this will probably clip/wrap if limiter is not working
    outputGen = sumOfFilters * (1 + q_v * 0.05);
    
  }
};

registerSynth(FilteredNoiseSynth);

#endif
