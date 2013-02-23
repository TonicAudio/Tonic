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
    
    Tonic::Generator rCarrierFreq = RampedValue(mtof(24)).target( registerMessage("carrierFreq", mtof(24)) );
    Tonic::Generator rModFreq     = rCarrierFreq * RampedValue(2).target( registerMessage("mcRatio", 2) );
    
    outputGen = SineWave().freq(
                  rCarrierFreq  + (
                    SineWave().freq(
                        rModFreq
                    ) * rModFreq * RampedValue(0).target( registerMessage("modIndex", 0) )
                  )
                ) * 0.5f;
  }
  
  static SourceRegister<FMDroneSynth> reg;
};


SourceRegister<FMDroneSynth> FMDroneSynth::reg("FMDroneSynth");

#endif
