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
#import "ControlStepper.h"
#import "ControlMetro.h"

using namespace Tonic;

class ControlSnapToScaleTestSynth : public Synth{
  public:
  ControlSnapToScaleTestSynth(){
   
    vector<float> scale = {0,2,3,7,10};
    
    ControlMetro metro = ControlMetro().bpm(350);
    
    ADSR env = ADSR(0.01, 1.4, 0, 0)
      .trigger(metro)
      .doesSustain(false)
      .legato(true);
    
    
    
    ControlStepper stepper1 = ControlStepper()
      .start(43)
      .end(90)
      .step(
        ControlStepper().start(4).end(11).step(1).trigger(metro)
      )
      .trigger(metro);
   
    ControlSnapToScale scaleSnapper1 =  ControlSnapToScale()
      .setScale(scale)
      .in( stepper1 );
    
    
    
    
    ControlStepper stepper2 = ControlStepper()
      .start(43)
      .end(92)
      .step(
        ControlStepper().start(4).end(13).step(1).trigger(metro)
      )
      .trigger(metro);
   
    ControlSnapToScale scaleSnapper2 =  ControlSnapToScale()
      .setScale(scale)
      .in( stepper2 );
    
    
  
    outputGen = (
    
      SineWave().freq(
        ControlMidiToFreq().in(scaleSnapper1)
      )
  
      +
      
      SineWave().freq(
        ControlMidiToFreq().in(scaleSnapper2 + 24)
      ) * 0.1
      
    )
    
    * 0.3 * env >> StereoDelay(0.1,0.11).feedback(0.3);
    
    
  }
};


registerSynth(ControlSnapToScaleTestSynth);

#endif
