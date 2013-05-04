//
//  RectWaveTestSynth.h
//  TonicDemo
//
//  Created by Morgan Packard on 2/28/13.

//

/*
Because of the fancy registerSynth macro, and because this synth has the same interface
as Synth it's not necessary to create a header file. You can create an instance of this synth using 
SynthFactory::createInstance(SYNTH_NAME), where SYNTH_NAME is the name supplied to registerSynth.
*/

#ifndef TonicDemo_RectWaveTestSynth_h
#define TonicDemo_RectWaveTestSynth_h

#include "Tonic.h"

using namespace Tonic;

class RectWaveTestSynth : public Synth {
  
public:
  RectWaveTestSynth(){
    
    Generator rectWave = RectWave().freq( addParameter("freq", 400).smoothed() ).pwm( addParameter("pwm", 0.5).smoothed() );
    LPF24 lfoFilter = LPF24().cutoff( SineWave().freq( 3.0 ) * 1800 + 2000 ).Q(1.5);
    outputGen = (rectWave >> lfoFilter) * 0.25;
  }
  
};

registerSynth(RectWaveTestSynth);

#endif
