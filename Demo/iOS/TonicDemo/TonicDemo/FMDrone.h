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
#include "Multiplier.h"
#include "Adder.h"
#include "FixedValue.h"
#include "RampedValue.h"

using namespace Tonic;

class FMDrone : public Synth {
  

public:
  FMDrone(){
    outputGen = SineWave().freq(300);
  }
};


SourceRegister<FMDrone> FMDrone::reg("FMDroneSynth");

#endif
