//
//  FlexToothTestSynth.h
//  TonicDemo
//
//  Created by Nick Donaldson on 3/2/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_FlexToothTestSynth_h
#define TonicDemo_FlexToothTestSynth_h

#include "Synth.h"
#include "FlexToothWave.h"
#include "SineWave.h"

class FlexToothTestSynth : public Synth {
  
public:
  
  FlexToothTestSynth(){
    
    FlexToothWave flexTooth = FlexToothWave().freq( addParameter("freq", 0.5).ramped() ).slope( addParameter("slope", 0).ramped() );
    outputGen = flexTooth * 0.2;
    
  }
  
  
};

registerSynth(FlexToothTestSynth);

#endif
