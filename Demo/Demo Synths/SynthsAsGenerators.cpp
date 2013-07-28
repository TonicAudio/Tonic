//
//  SynthsAsGenerators.cpp
//  TonicDemo
//
//  Created by Nick Donaldson on 5/23/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef __SynthsAsGenerators_cpp__
#define __SynthsAsGenerators_cpp__

#include "Tonic.h"

using namespace Tonic;

class SynthsAsGeneratorsDemoSynth : public Synth
{
  public:
  
    SynthsAsGeneratorsDemoSynth(){
      
      // Add two synths together and put them both through some delay
      Synth noiseSynth = SynthFactory::createInstance("FilteredNoiseSynth");
      Synth octaverSynth = SynthFactory::createInstance("ControlSnapToScaleTestSynth");
      
      // get all the control parameters and add them to this synth
      addParametersFromSynth(noiseSynth);
      addParametersFromSynth(octaverSynth);
      
      StereoDelay delay = StereoDelay(0.5f,0.5f).delayTimeLeft(0.2).delayTimeRight(0.3).dryLevel(1.0f).wetLevel(0.3f).feedback(0.3f);
      
      setOutputGen(((noiseSynth * 0.5 + octaverSynth) >> delay) * 0.8 );
      
    }
  
  
};

TONIC_REGISTER_SYNTH(SynthsAsGeneratorsDemoSynth);

#endif