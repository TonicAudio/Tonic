//
//  FMDrone.h
//  TonicDemo
//
//  Created by Morgan Packard on 2/15/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

/*
Because of the fancy registerSynth macro, and because this synth has the same interface
as Synth it's not necessary to create a header file. You can create an instance of this synth using 
SynthFactory::createInstance(SYNTH_NAME), where SYNTH_NAME is the name supplied to registerSynth.
*/

#ifndef TonicDemo_FMDrone_h
#define TonicDemo_FMDrone_h

#include "Synth.h"
#include "SineWave.h"

using namespace Tonic;

class FMDroneSynth : public Synth {
  

public:
  FMDroneSynth(){
    
      Generator rCarrierFreq = (addParameter("carrierFreq", 0) * 2 + 30).ramped();
      Generator rModFreq     = rCarrierFreq * addParameter("mcRatio", 2).ramped();
      
      outputGen = SineWave().freq(
                                  rCarrierFreq  + (
                                                   SineWave().freq( rModFreq ) *
                                                   rModFreq *
                                                   (addParameter("modIndex", 0) * 10.0f).ramped()
                                                   )
                                  ) * 0.5f;
  }
  
};

registerSynth(FMDroneSynth);

#endif
