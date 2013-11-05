//
//  FilteredNoise.h
//  TonicDemo
//
//  Created by Morgan Packard on 2/24/13.

//

/*
Because of the fancy TONIC_REGISTER_SYNTH macro, and because this synth has the same interface
as Synth it's not necessary to create a header file. You can create an instance of this synth using 
SynthFactory::createInstance(SYNTH_NAME), where SYNTH_NAME is the name supplied to TONIC_REGISTER_SYNTH.
*/

#ifndef TonicDemo_FilteredNoise_h
#define TonicDemo_FilteredNoise_h

#include "Tonic.h"

using namespace Tonic;

class FilteredNoiseSynth : public Synth{
public:
  FilteredNoiseSynth(){
    vector<float> midiNums;
    midiNums.push_back(-24);
    midiNums.push_back(0);
    midiNums.push_back(3);
    midiNums.push_back(7);
    midiNums.push_back(10);
    
    const float cutoffMult = 100;
    
    PinkNoise noise = PinkNoise();
    
    ControlGenerator cutoffCtrl = addParameter("cutoff", 0.5);
    
    Generator q_v = addParameter("Q", 5).smoothed();
        
    ControlGenerator lowBasFreq =   ControlFloor().input( midiNums.at(0) + 12 + cutoffCtrl * cutoffMult ) >> ControlMidiToFreq();
    ControlGenerator fmAmt = ControlValue(0.1);
    Generator cutoffSlowSwell = (SineWave().freq(0.1) + 1.0f) * 500;
    cutoffSlowSwell = FixedValue(500);
    Generator lpfCutoff = 100 + (cutoffSlowSwell + 0.5 * SineWave().freq(10) * cutoffSlowSwell);
    LPF12 filter = LPF12().cutoff(lpfCutoff);
    
    ControlGenerator toothyBassRandomAmp = ControlRandom()
        .min(-0.1)
        .max(1)
        .trigger(
            ControlMetro().bpm( ControlRandom().min(10).max(30) )
        );
    
    Generator toothyBassSwell = ( (toothyBassRandomAmp * toothyBassRandomAmp * toothyBassRandomAmp * 5) >> ControlPrinter().message("toothyBassSwell %f") ).smoothed(10);
    
    Generator lowToothyBass =
      RectWave()
      .freq(  lowBasFreq )
      .pwm(  0.5 + 0.2 * (SineWave().freq(0.013) + 1)  );
    
    lowToothyBass = filter.input( lowToothyBass );
    lowToothyBass = lowToothyBass * toothyBassSwell;
    
    Adder sumOfFilters;
    
    for(int i = 0; i < midiNums.size(); i++){
      Generator tremelo = (SineWave().freq( randomFloat(0.1, 0.3) ) + 1.5) * 0.3;
      Generator cutoff = ControlMidiToFreq().input( ControlFloor().input( midiNums.at(i) + cutoffCtrl * cutoffMult  )).smoothed().length(0.01);
      BPF24 filter = BPF24().Q( q_v ).cutoff( cutoff ).normalizesGain(true);
      sumOfFilters.input((noise >> filter) * tremelo);
    }
    
    // add a bit of gain for higher Q
    // Using this to test output limiter as well - this will probably clip/wrap if limiter is not working
    setOutputGen(
      sumOfFilters * (1 + q_v * 0.05)
      +
      lowToothyBass * 0.05
    );
    
    
  }
};

TONIC_REGISTER_SYNTH(FilteredNoiseSynth);

#endif
