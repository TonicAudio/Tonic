//
//  FlexToothLFOTestSynth.h
//  TonicDemo
//
//  Created by Nick Donaldson on 3/2/13.

//

/*
Because of the fancy registerSynth macro, and because this synth has the same interface
as Synth it's not necessary to create a header file. You can create an instance of this synth using 
SynthFactory::createInstance(SYNTH_NAME), where SYNTH_NAME is the name supplied to registerSynth.
*/

#ifndef TonicDemo_FlexToothLFOTestSynth_h
#define TonicDemo_FlexToothLFOTestSynth_h

#include "Synth.h"
#include "FlexToothWave.h"
#include "SineWave.h"

using namespace Tonic;

class FlexToothLFOTestSynth : public Synth {
  
public:
  
  FlexToothLFOTestSynth(){
    
    FlexToothWave lfo = FlexToothWave().freq( addParameter("freq", 0.5).smoothed() ).slope( addParameter("slope", 0).smoothed() );
    outputGen = SineWave().freq( 300 + ((lfo * 0.5 + 1) * 200) ) * 0.2;
    
  }
  
  
};

registerSynth(FlexToothLFOTestSynth);

#endif
