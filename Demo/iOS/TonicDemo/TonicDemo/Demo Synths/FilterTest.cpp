//
//  FilterTest.h
//  TonicDemo
//
//  Created by Nick Donaldson on 2/7/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

/*
Because of the fancy registerSynth macro, and because this synth has the same interface
as Synth it's not necessary to create a header file. You can create an instance of this synth using 
SynthFactory::createInstance(SYNTH_NAME), where SYNTH_NAME is the name supplied to registerSynth.
*/

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
    
    ControlValue cutoff = addParameter("cutoff", 1000, 20, 20000);
    ControlValue lfo = addParameter("LFO", 0, 0, 10);
    
    Generator cutoffRamped = cutoff.ramped();
    
    LPF24 lpf = LPF24().Q(2.5).cutoff( cutoffRamped + (SineWave().freq(4) * cutoffRamped * 0.2 * lfo.ramped() ) );
    
    // Need a limiter class soon - these tend to clip with high-Q
    outputGen = (Noise() >> lpf) * 0.5;
  }
  
};

registerSynth(FilterTest);

#endif
