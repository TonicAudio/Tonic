//
//  CombFilterTestSynth.cpp
//  TonicDemo
//
//  Created by Nick Donaldson on 5/4/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//


#include "Tonic.h"

using namespace Tonic;

class CombFilterTestSynth : public Synth
{
public:
  
  CombFilterTestSynth() {
    
    outputGen = (Noise() >> FFCombFilter(0.1,0.5).delayTime(addParameter("delayTime").smoothed())).scaleFactor(0.8) * 0.2;
    
  }
  
};

registerSynth(CombFilterTestSynth);