//
//  ADSRTestSynth.h
//  TonicDemo
//
//  Created by Morgan Packard on 2/28/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_ADSRTestSynth_h
#define TonicDemo_ADSRTestSynth_h

#include "Synth.h"
#include "Noise.h"
#include "ADSR.h"

using namespace Tonic;

class ADSRTestSynth : public Synth {
  
public:
  ADSRTestSynth(){

    outputGen = ADSR()
      .setTrigger( addParameter("trigger") )
      .setAttack(true)
      .setDecay(0.5)
      .setSustain(0.5)
      .setRelease(0.5)
      .setDoesSustain(true)
       * Noise() * 0.1;
  }
  
};

registerSynth(ADSRTestSynth);
#endif
