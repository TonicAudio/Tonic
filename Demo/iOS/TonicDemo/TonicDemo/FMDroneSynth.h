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
       RampedValue(200).target( registerMessage("baseFreq", 200) ).lengthMs(100)
      + (
          SineWave().freq( 2 * RampedValue(200).target( registerMessage("baseFreq") ) )
          * 10 * RampedValue(1).target(registerMessage("fmAmount", 1)).lengthMs(100)
         )
    );
  }
  
  static SourceRegister<FMDroneSynth> reg;
};


SourceRegister<FMDroneSynth> FMDroneSynth::reg("FMDroneSynth");

#endif
