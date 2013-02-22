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
    
    // Need a limiter class soon - these tend to clip with high-Q
    outputGen = BPF24().input( Noise() ).Q(10).cutoff(
                  RampedValue(1000).target( registerMessage("cutoff",1000) )
                  + (SineWave().freq(4) * RampedValue(0).target( registerMessage("LFO", 0) ) )
                ) * 0.5;
  }
  
  static SourceRegister<FilterTest> reg;
};


SourceRegister<FilterTest> FilterTest::reg("FilterTest");

#endif
