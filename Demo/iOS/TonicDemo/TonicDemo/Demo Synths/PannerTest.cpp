//
//  PannerTest.h
//  TonicDemo
//
//  Created by Morgan Packard on 3/7/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

/*
Because of the fancy registerSynth macro, and because this synth has the same interface
as Synth it's not necessary to create a header file. You can create an instance of this synth using 
SynthFactory::createInstance(SYNTH_NAME), where SYNTH_NAME is the name supplied to registerSynth.
*/

#ifndef TonicDemo_PannerTest_h
#define TonicDemo_PannerTest_h

#include "Synth.h"
#include "Noise.h"
#include "MonoToStereoPanner.h"

using namespace Tonic;

class PannerTest : public Synth{
  
public:
  
  PannerTest () {
    
    outputGen =  MonoToStereoPanner().pan( addParameter("pan", 1, -1, 1) ).input( Noise() * 0.4 );

  }

};


registerSynth(PannerTest);

#endif
