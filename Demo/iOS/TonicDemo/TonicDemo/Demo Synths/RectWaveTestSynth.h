//
//  RectWaveTestSynth.h
//  TonicDemo
//
//  Created by Morgan Packard on 2/28/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_RectWaveTestSynth_h
#define TonicDemo_RectWaveTestSynth_h

#include "Synth.h"
#include "RectWave.h"
#include "Filters.h"

using namespace Tonic;

class RectWaveTestSynth : public Synth {
  
public:
  RectWaveTestSynth(){
    
    Generator rectWave = RectWave().freq( addParameter("freq", 400).ramped() ).pwm( addParameter("pwm", 0.5).ramped() );
    LPF24 lfoFilter = LPF24().cutoff( SineWave().freq( 3.0 ) * 1800 + 2000 ).Q(1.5);
    outputGen = (rectWave >> lfoFilter) * 0.25;
  }
  
};

registerSynth(RectWaveTestSynth);

#endif
