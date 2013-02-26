//
//  FilteredNoise.h
//  TonicDemo
//
//  Created by Morgan Packard on 2/24/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_FilteredNoise_h
#define TonicDemo_FilteredNoise_h

#include "Tonic.h"
#include "Synth.h"
#include "SineWave.h"
#include "Filters.h"
#include "RampedValue.h"

using namespace Tonic;

class FilteredNoiseSynth : public Synth{
public:
  FilteredNoiseSynth(){
    vector<float> midiNums;
    midiNums.push_back(-24);
    midiNums.push_back(0);
    midiNums.push_back(3);
    midiNums.push_back(7);
    midiNums.push_back(10);
    Noise noise = Noise();
    
    ControlValue cutoff0 = registerMessage("cutoff", 1);
    
    for(int i = 0; i < midiNums.size(); i++){
      Generator tremelo = (SineWave().freq( randomFloat(0.1, 0.3) ) + 1.5) * 0.3;
      Generator cutoff = registerMessage("cutoff", 1).ramped() * mtof(midiNums.at(i) + 80);
      BPF24 filter = BPF24().Q( registerMessage("Q", 5) ).cutoff( cutoff );
      outputGen = outputGen + (noise >> filter) * tremelo;
    }
    
  }
};

registerSynth(FilteredNoiseSynth);

#endif
