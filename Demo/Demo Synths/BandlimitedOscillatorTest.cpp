//
//  BandlimitedOscillatorTest.cpp
//  TonicDemo
//
//  Created by Nick Donaldson on 7/21/13.
//

#ifndef TonicDemo_BandlimitedOscillatorTest_cpp
#define TonicDemo_BandlimitedOscillatorTest_cpp

#include "Tonic.h"

using namespace Tonic;

class BandlimitedOscillatorTest : public Synth
{

public:
  
  BandlimitedOscillatorTest()
  {
    ControlParameter blend = addParameter("blend").min(0).max(1);
  
    Generator freqSweep = SineWave().freq(0.25) * 1000 + 1080;
    Generator smoothBlend = blend.smoothed();
    
    Generator output = (SquareWave().freq(freqSweep) * (1.0 - smoothBlend)) + (SquareWaveBL().freq(freqSweep) * smoothBlend);
    setOutputGen(output * 0.25);
  }
  
};

TONIC_REGISTER_SYNTH(BandlimitedOscillatorTest);

#endif