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
    outputGen = SineWave().freq(
       RampedValue(200).target( registerMessage("baseFreq", 200) )
      + (
          SineWave().freq( 10 ) * RampedValue(0).target(registerMessage("fmAmount", 0))
         )
    );
  }
  
  static SourceRegister<FMDroneSynth> reg;
};


SourceRegister<FMDroneSynth> FMDroneSynth::reg("FMDroneSynth");

#endif
