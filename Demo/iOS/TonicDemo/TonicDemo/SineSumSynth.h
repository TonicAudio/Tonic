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
    
    SynthParameter pitch = addParameter("pitch",0,0,1);
      
      Adder outputAdder;
    
    for (int s=0; s<NUM_SINES; s++){
      
      ControlGenerator pitchGen = ((pitch.value * 220 + 220) * powf(2, (s - (NUM_SINES/2)) * 5.0f / 12.0f));
            
      outputAdder = outputAdder + SineWave().freq( pitchGen.ramped() );
      
    }
    
    outputGen = outputAdder * ((1.0f/NUM_SINES) * 0.5f);

  }

};

registerSynth(SineSumSynth);

#endif
