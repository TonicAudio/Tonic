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
    
    Tonic::Generator cutoff = registerMessage("cutoff",1000).ramped();

    LPF24 lpf = LPF24().Q(2.5).cutoff( cutoff + (SineWave().freq(4) * cutoff * 0.2 * registerMessage("LFO", 0).ramped() ) );
    
    // Need a limiter class soon - these tend to clip with high-Q
    outputGen = (Noise() >> lpf) * 0.5;
  }
  
};

registerSynth(FilterTest);

#endif
