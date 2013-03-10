//
//  PannerTest.h
//  TonicDemo
//
//  Created by Morgan Packard on 3/7/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_PannerTest_h
#define TonicDemo_PannerTest_h

#include "Synth.h"
#include "Noise.h"
#include "MonoToStereoPanner.h"

using namespace Tonic;

class PannerTest : public Synth{
  
public:
  
  PannerTest () {
    
    outputGen =  MonoToStereoPanner().pan( addParameter("pan", 1, -1, 1) ).input( Noise() );

  }

};


registerSynth(PannerTest);

#endif
