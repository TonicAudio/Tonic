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
    
    Tonic::Generator rCarrierFreq = registerMessage("carrierFreq", mtof(24)).ramped();
    Tonic::Generator rModFreq     = rCarrierFreq * registerMessage("mcRatio", 2).ramped();
    
    outputGen = SineWave().freq(
                  rCarrierFreq  + (SineWave().freq( rModFreq ) * rModFreq * registerMessage("modIndex", 0).ramped())
                  
                ) * 0.5f;
  }
  
  static SourceRegister<FMDroneSynth> reg;
};


SourceRegister<FMDroneSynth> FMDroneSynth::reg("FMDroneSynth");

#endif
