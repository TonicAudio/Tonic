//
//  StereoDelayTestSynth.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 3/13/13.

//

#ifndef TonicDemo_StereoDelayTestSynth_cpp
#define TonicDemo_StereoDelayTestSynth_cpp

#include "Tonic.h"

using namespace Tonic;

class StereoDelayTestSynth : public Synth {
  
public:
  StereoDelayTestSynth(){
    
    ControlMetro metro = ControlMetro().bpm(100);
    ControlGenerator freq = ControlRandom().trigger(metro).min(0).max(1);
  
    Generator tone = RectWave().freq(
        freq * addParameter("frequencyRandomAmount") + 100
        + addParameter("freq")
      ) * SineWave().freq(50);
    
    ADSR env = ADSR()
      .attack(0.01)
      .decay( addParameter("decay") )
      .sustain(0)
      .release(0)
      .doesSustain(false)
      .trigger(metro);
    
    StereoDelay delay = StereoDelay(3.0f,3.0f)
    .delayTimeLeft( 0.5 + SineWave().freq(0.2) * 0.01)
    .delayTimeRight(0.55 + SineWave().freq(0.23) * 0.01)
    .feedback(0.3)
    .dryLevel(0.8)
    .wetLevel(0.2);
    
    Generator filterFreq = (SineWave().freq(0.01) + 1) * 200 + 225;
    
    LPF24 filter = LPF24().Q(2).cutoff( filterFreq );

    Generator outputGen = filter.input(delay.input( tone * env )) * 0.3;
    
    setOutputGen(outputGen);
  }
  
  
};

TONIC_REGISTER_SYNTH(StereoDelayTestSynth)

#endif
