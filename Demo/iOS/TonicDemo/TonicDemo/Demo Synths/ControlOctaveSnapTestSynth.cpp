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
#import "ADSR.h"

using namespace Tonic;

class ControlSnapToScaleTestSynth : public Synth{
  public:
  ControlSnapToScaleTestSynth(){
  
    ControlSnapToScale scaleSnapper =  ControlSnapToScale()
      .setScale({0,2,3,7,10})
      .in( addParameter("pitch") * 80 + 10 );
   
    ADSR env = ADSR(0.01, 0.1, 0, 0)
      .trigger(scaleSnapper)
      .doesSustain(false)
      .legato(true);
  
    outputGen = SineWave().freq(
      ControlMidiToFreq().in(scaleSnapper)
    ) * 0.3 * env >> StereoDelay(0.1,0.11).feedback(0.3);
  }
};


registerSynth(ControlSnapToScaleTestSynth);

#endif
