//
//  FlexToothLFOTestSynth.h
//  TonicDemo
//
//  Created by Nick Donaldson on 3/2/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_FlexToothLFOTestSynth_h
#define TonicDemo_FlexToothLFOTestSynth_h

#include "Synth.h"
#include "FlexToothWave.h"
#include "SineWave.h"

class FlexToothLFOTestSynth : public Synth {
  
public:
  
  FlexToothLFOTestSynth(){
    
    FlexToothWave lfo = FlexToothWave().freq( addParameter("freq", 0.5).ramped() ).slope( addParameter("slope", 0).ramped() );
    outputGen = SineWave().freq( 300 + ((lfo * 0.5 + 1) * 200) ) * 0.2;
    
  }
  
  
};

registerSynth(FlexToothLFOTestSynth);

#endif
