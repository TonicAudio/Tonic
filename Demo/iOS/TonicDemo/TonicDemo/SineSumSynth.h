//
//  SineSumSynth.h
//  TonicDemo
//
//  Created by Nick Donaldson on 2/7/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_SineSumSynth_h
#define TonicDemo_SineSumSynth_h

#define NUM_SINES 10

#include "Synth.h"
#include "SineWave.h"

using namespace Tonic;

class SineSumSynth : public Synth{
  
public:
  
  SineSumSynth () {
    
    SynthParameter spread = addParameter("spread",0,0,1);
    
    for (int s=0; s<NUM_SINES; s++){
      
      ControlGenerator spreadGen = (spread.value * powf(2,s - (NUM_SINES/2)));
            
      outputGen = outputGen + SineWave().freq( 440 * spreadGen.ramped() );
      
    }
    
    outputGen = outputGen * ((1.0f/NUM_SINES) * 0.5f);

  }

};

registerSynth(SineSumSynth);

#endif
