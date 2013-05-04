//
//  ControlSnapToScaleTestSynth.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 3/24/13.

//

#ifndef TonicDemo_ControlSnapToScaleTestSynth_cpp
#define TonicDemo_ControlSnapToScaleTestSynth_cpp

#import "Tonic.h"

using namespace Tonic;

class ControlSnapToScaleTestSynth : public Synth{
  public:
  ControlSnapToScaleTestSynth(){
   
    float scaleArr[6] = {0,2,3,7,10};
    vector<float> scale(scaleArr, scaleArr + sizeof scaleArr / sizeof scaleArr[0]);
    
    ControlGenerator speed = addParameter("speed", 0.85);
    
    ControlMetro switchIt = ControlMetro().bpm(30 * speed);
    
    ControlMetro metro = ControlMetro().bpm(
        ControlStepper()
          .bidirectional(1)
          .start(550)
          .end(650)
          .step(100)
          .trigger(switchIt)
        * speed
      );
    
    
    ADSR env = ADSR(0.01, 0.5, 0, 0)
      .trigger(metro)
      .doesSustain(false)
      .legato(true)
      .decay(
        ControlStepper()
        .start(0.05)
        .end(0.5)
        .step(0.01)
        .trigger(metro)
      );
    
    ControlGenerator stepperStart = addParameter("stepperStart") * 30 + 43;
    
    ControlStepper stepper1 = ControlStepper()
    .bidirectional(1)
    .start( stepperStart )
    .end(stepperStart + 50 *  addParameter("stepperSpread"))
    .step(
      ControlStepper()
      .bidirectional(1)
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
        ControlMidiToFreq().in(scaleSnapper1 + -12).smoothed().length(
          ControlRandom().min(0).max(0.03).trigger(switchIt)
        )
      ) 
      
      +
    
      SineWave().freq(
        ControlMidiToFreq().in(scaleSnapper1 + 12)
      ) * 0.1
      
    )
    
    * 0.3 * env >> StereoDelay(0.3,0.33).feedback(0.3).mix(
      ControlRandom().min(0).max(0.1).trigger(switchIt).smoothed()
    );
    
    
  }
};


registerSynth(ControlSnapToScaleTestSynth);

#endif
