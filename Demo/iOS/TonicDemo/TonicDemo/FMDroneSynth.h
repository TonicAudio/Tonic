//
//  FMDrone.h
//  TonicDemo
//
//  Created by Morgan Packard on 2/15/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//



#ifndef TonicDemo_FMDrone_h
#define TonicDemo_FMDrone_h

#include "Synth.h"
#include "SineWave.h"

using namespace Tonic;

class FMDroneSynth : public Synth {
  

public:
  FMDroneSynth(){
    
    SynthParameter & mcRatio = addParameter("mcRatio", SynthParameterTypeIncremental);
    mcRatio.min = 1;
    mcRatio.max = 10;
    mcRatio.increment = 1;
    
    Generator rCarrierFreq = addParameter("carrierFreq", 60).value.ramped();
    Generator rModFreq     = rCarrierFreq * mcRatio.value.ramped();
    
    outputGen = SineWave().freq(
                  rCarrierFreq  + (
                    SineWave().freq( rModFreq ) *
                    rModFreq *
                    addParameter("modIndex", 0).value.ramped()
                  )
                ) * 0.5f;
  }
  
};

registerSynth(FMDroneSynth);

#endif
