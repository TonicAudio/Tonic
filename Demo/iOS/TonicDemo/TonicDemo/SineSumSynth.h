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

#include "Tonic.h"
#include "Synth.h"
#include "SineWave.h"

using namespace Tonic;

class SineSumSynth : public Synth{
  
public:
  
  SineSumSynth () {
    
    for (int s=0; s<NUM_SINES; s++){
      sineMixer.in(sines[s].freq(sineFreqs[s].value(440).lengthMs(100)));
    }
    
    outputGen =
    (
      sineMixer
    ) * (1.0f/NUM_SINES) * 0.5f;

    
  }
  
  inline void setSpread(TonicFloat spread){
    for (int i=0; i<NUM_SINES; i++){
      // spread up to an octave apart
      TonicFloat freq = 440.0f * powf(2, spread*(i - (NUM_SINES/2)));
      sineFreqs[i].target(freq);
    }
  }
  
private:
  
  SineWave sines[NUM_SINES];
  RampedValue sineFreqs[NUM_SINES];
  Adder sineMixer;
    
};

registerSynth(SineSumSynth);

#endif
