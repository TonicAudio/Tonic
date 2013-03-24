//
//  ControlOctaveSnapTestSynth.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 3/24/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_ControlOctaveSnapTestSynth_cpp
#define TonicDemo_ControlOctaveSnapTestSynth_cpp

#import "Synth.h"
#import "ControlOctaveSnap.h"
#import "SineWave.h"
#import "ControlMidiToFreq.h"
#import "ControlOctaveSnap.h"

using namespace Tonic;

class ControlOctaveSnapTestSynth : public Synth{
  public:
  ControlOctaveSnapTestSynth(){
    outputGen = SineWave().freq(
      ControlMidiToFreq().in(
        ControlOctaveSnap().setScale({0,2,3,7,10})
        .in(
          addParameter("pitch") * 80 + 10
        )
      )
    );
  }
};


registerSynth(ControlOctaveSnapTestSynth);

#endif
