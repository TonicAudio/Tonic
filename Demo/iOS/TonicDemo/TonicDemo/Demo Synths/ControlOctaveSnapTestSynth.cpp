//
//  ControlSnapToScaleTestSynth.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 3/24/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_ControlSnapToScaleTestSynth_cpp
#define TonicDemo_ControlSnapToScaleTestSynth_cpp

#import "Synth.h"
#import "ControlSnapToScale.h"
#import "SineWave.h"
#import "ControlMidiToFreq.h"
#import "ControlSnapToScale.h"
#import "StereoDelay.h"

using namespace Tonic;

class ControlSnapToScaleTestSynth : public Synth{
  public:
  ControlSnapToScaleTestSynth(){
    outputGen = SineWave().freq(
      ControlMidiToFreq().in(
        ControlSnapToScale().setScale({0,2,3,7,10})
        .in(
          addParameter("pitch") * 80 + 10
        )
      )
    ) * 0.2 >> StereoDelay(1,1.1).feedback(0.1);
  }
};


registerSynth(ControlSnapToScaleTestSynth);

#endif
