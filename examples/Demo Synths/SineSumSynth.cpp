//
//  SineSumSynth.h
//  TonicDemo
//
//  Created by Nick Donaldson on 2/7/13.

//

/*
Because of the fancy TONIC_REGISTER_SYNTH macro, and because this synth has the same interface
as Synth it's not necessary to create a header file. You can create an instance of this synth using 
SynthFactory::createInstance(SYNTH_NAME), where SYNTH_NAME is the name supplied to TONIC_REGISTER_SYNTH.
*/

#ifndef TonicDemo_SineSumSynth_h
#define TonicDemo_SineSumSynth_h

#define NUM_SINES 10

#include "Tonic.h"

using namespace Tonic;

class SineSumSynth : public Synth{
  
public:
  
  SineSumSynth () {
    
    ControlParameter pitch = addParameter("pitch",0);
      
    Adder outputAdder;
    
    for (int s=0; s<NUM_SINES; s++){
      
      ControlGenerator pitchGen = ((pitch * 220 + 220) * powf(2, (s - (NUM_SINES/2)) * 5.0f / 12.0f));
            
      outputAdder.input(SineWave().freq( pitchGen.smoothed() ));
      
    }
    
    Generator outputGen = outputAdder * ((1.0f/NUM_SINES) * 0.5f);
    
    setOutputGen(outputGen);

  }

};

TONIC_REGISTER_SYNTH(SineSumSynth);

#endif
