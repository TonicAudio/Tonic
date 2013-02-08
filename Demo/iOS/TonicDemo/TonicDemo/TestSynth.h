//
//  TestSynth.h
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/23/13.
//
//

#ifndef _013_1_23_melody_TestSynth_h
#define _013_1_23_melody_TestSynth_h

#include "Tonic.h"
#include "Synth.h"
#include "SineWave.h"
#include "MonoToStereo.h"
#include "Multiplier.h"
#include "FixedValue.h"
#include "RampedValue.h"
#include "Noise.h"
#include "Adder.h"

using namespace Tonic;

class TestSynth : public Synth{

public:

  TestSynth() : carrierAmt(1.0f) {
    
    carrierFreq.defValue(10).defLenMs(2000);
    modFreq.defValue(400);
    
    carrier.freq(carrierFreq);
    modulator.freq(modFreq);
      
    outputGen =
    (
      ( modulator + carrierAmt ) * carrier
    ) * 0.1f;
    
  };
  
  inline void setCarrierAmt(TonicFloat newCarrierAmt) { carrierAmt = clamp(newCarrierAmt, 0.0f, 1.0f); };
  inline void setCarrierFreq(TonicFloat nCarFreq) { carrierFreq.setTarget(nCarFreq); };
  inline void setModFreq(TonicFloat nModFreq) { modFreq.setTarget(nModFreq); };
  
private:
  
  SineWave carrier;
  SineWave modulator;
  
  RampedValue carrierFreq;
  RampedValue modFreq;
  
  TonicFloat carrierAmt;
  
};

#endif
