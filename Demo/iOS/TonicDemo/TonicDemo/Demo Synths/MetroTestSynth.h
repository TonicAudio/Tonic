//
//  MetroTestSynth.h
//  TonicDemo
//
//  Created by Nick Donaldson on 3/10/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_MetroTestSynth_h
#define TonicDemo_MetroTestSynth_h

#include "Synth.h"
#include "Noise.h"
#include "ADSR.h"
#include "ControlMetro.h"
#include "ControlPulse.h"

using namespace Tonic;

class MetroTestSynth : public Synth{
  
public:
  
  MetroTestSynth(){
    
    ControlValue bpm = addParameter("bpm", 120, 60, 5000);
    ControlMetro metro = ControlMetro().bpm(bpm);
    ControlPulse pulseGate = ControlPulse().in(metro).length(bpm * (1.0f/120.0f));
    
    ADSR envelope = ADSR().attack(0.001).decay(0.0).sustain(1.0).release(0.025).doesSustain(false);
    
    outputGen = Noise() * envelope.trigger(pulseGate) * 0.25;
  }
  
};

registerSynth(MetroTestSynth);

#endif
