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
#import "ControlRandom.h"

using namespace Tonic;

class ControlSnapToScaleTestSynth : public Synth{
  public:
  ControlSnapToScaleTestSynth(){
   
    vector<float> scale = {0,2,3,7,10};
    
    ControlMetro switchIt = ControlMetro().bpm(30);
    
    ControlMetro metro = ControlMetro().bpm(
        ControlStepper()
          .start(550)
          .end(650)
          .step(100)
          .trigger(switchIt)
      );
    
    ADSR env = ADSR(0.01, 0.5, 0, 0)
      .trigger(metro)
      .doesSustain(false)
      .legato(true)
      .decay(
        ControlStepper()
        .start(0.1)
        .end(10)
        .step(10.9)
        .trigger(ControlMetro().bpm(30))
      );
    
    ControlGenerator stepperStart = addParameter("stepperStart") * 30 + 43;
    
    ControlStepper stepper1 = ControlStepper()
    .start( stepperStart )
    .end(stepperStart + 50 *  addParameter("stepperSpread"))
    .step(
      ControlStepper()
      .start(4)
      .end(
        ControlRandom()
        .min(9)
        .max(15)
        .trigger(
          ControlMetro().bpm(11)
        )
      )
      .step(1)
      .trigger(metro)
    )
    .trigger(metro);
   
    ControlSnapToScale scaleSnapper1 =  ControlSnapToScale()
      .setScale(scale)
      .in( stepper1 );
    
    outputGen = (
    
      SineWave().freq(
        ControlMidiToFreq().in(scaleSnapper1 + -12).ramped().length(
          ControlRandom().min(0).max(0.03).trigger(switchIt)
        )
      ) 
      
      +
    
      SineWave().freq(
        ControlMidiToFreq().in(scaleSnapper1 + 12)
      ) * 0.1
      
    )
    
    * 0.3 * env >> StereoDelay(0.3,0.33).feedback(0.3).mix(
      ControlRandom().min(0).max(0.1).trigger(switchIt).ramped()
    );
    
    
  }
};


registerSynth(ControlSnapToScaleTestSynth);

#endif
