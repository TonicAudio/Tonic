//
//  FMDrone.h
//  TonicDemo
//
//  Created by Nick Donaldson on 2/15/13.

//

/*
Because of the fancy registerSynth macro, and because this synth has the same interface
as Synth it's not necessary to create a header file. You can create an instance of this synth using 
SynthFactory::createInstance(SYNTH_NAME), where SYNTH_NAME is the name supplied to registerSynth.
*/

#ifndef TonicDemo_FMDrone_h
#define TonicDemo_FMDrone_h

#include "Tonic.h"

using namespace Tonic;

class FMDroneSynth : public Synth {
  

public:
  FMDroneSynth(){
    
    ControlValue volume = addParameter("volume", "Volume (dbFS)", -12.f, -60.f, 0.f);
    ControlValue carrierPitch = addParameter("carrierPitch", "Carrier Pitch", 28.f, 20.f, 32.f);
    ControlValue modIndex = addParameter("modIndex", "FM Amount", 0.25f, 0.f, 1.0f);
    ControlValue lfoAmt = addParameter("lfoAmt", "LFO Amount", 0.5f, 0.f, 1.f);
    
    Generator rCarrierFreq = ControlMidiToFreq().in(carrierPitch).smoothed();
    Generator rModFreq     = rCarrierFreq * 4.0f;
      
      outputGen = SineWave()
        .freq( rCarrierFreq
          + (
            SineWave().freq( rModFreq ) *
            rModFreq *
             (modIndex.smoothed() * (1.0f + SineWave().freq((LFNoise().setFreq(0.5f) + 1.f) * 2.f + 0.2f) * (lfoAmt * 0.5f).smoothed()))
          )
        ) * ControlDbToLinear().in(volume).smoothed() * ((SineWave().freq(0.15f) + 1.f) * 0.75f + 0.25);
  }
  
};

registerSynth(FMDroneSynth);

#endif
