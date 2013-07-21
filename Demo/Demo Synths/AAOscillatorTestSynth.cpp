//
//  AAOscillatorTestSynth.cpp
//  TonicDemo
//
//  Created by Nick Donaldson on 7/21/13.
//

#ifndef TonicDemo_AAOscillatorTestSynth_cpp
#define TonicDemo_AAOscillatorTestSynth_cpp

#include "Tonic.h"

using namespace Tonic;

class AAOscillatorTestSynth : public Synth
{

public:
  
  AAOscillatorTestSynth()
  {
    Generator output = RectWaveBL().freq(400);
    setOutputGen(output * 0.25);
  }
  
};

registerSynth(AAOscillatorTestSynth);

#endif