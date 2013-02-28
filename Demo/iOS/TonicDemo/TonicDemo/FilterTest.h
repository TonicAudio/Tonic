//
//  FilterTest.h
//  TonicDemo
//
//  Created by Nick Donaldson on 2/7/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//



#ifndef TonicDemo_FilterTest_h
#define TonicDemo_FilterTest_h

#include "Synth.h"
#include "Noise.h"
#include "Filters.h"
#include "SineWave.h"

using namespace Tonic;

class FilterTest : public Synth {
  
  
public:
  FilterTest(){
    
    SynthParameter cutoff = addParameter("cutoff", 1000, 20, 20000);
    SynthParameter lfo = addParameter("LFO", 0, 0, 10);
    
    Generator cutoffRamped = cutoff.value.ramped();
    
    LPF24 lpf = LPF24().Q(2.5).cutoff( cutoffRamped + (SineWave().freq(4) * cutoffRamped * 0.2 * lfo.value.ramped() ) );
    
    // Need a limiter class soon - these tend to clip with high-Q
    outputGen = (Noise() >> lpf) * 0.5;
  }
  
};

registerSynth(FilterTest);

#endif
