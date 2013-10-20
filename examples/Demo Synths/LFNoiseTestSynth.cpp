//
//  LFNoiseTest.h
//  TonicDemo
//
//  Created by Morgan Packard on 2/26/13.

//

/*
Because of the fancy TONIC_REGISTER_SYNTH macro, and because this synth has the same interface
as Synth it's not necessary to create a header file. You can create an instance of this synth using 
SynthFactory::createInstance(SYNTH_NAME), where SYNTH_NAME is the name supplied to TONIC_REGISTER_SYNTH.
*/

#ifndef TonicDemo_LFNoiseTest_h
#define TonicDemo_LFNoiseTest_h

#include "Tonic.h"

using namespace Tonic;

class LFNoiseTestSynth : public Synth{
public:
  LFNoiseTestSynth(){

    Generator outputGen = SineWave().freq(
      500 + 500 * LFNoise().setFreq(
        addParameter("noiseFreq", 100) )
      )
       * 0.3;
    
    setOutputGen(outputGen);
  }
};

TONIC_REGISTER_SYNTH(LFNoiseTestSynth);


#endif
