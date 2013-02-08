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
#include "Adder.h"

using namespace Tonic;

class TestSynth : public Synth{

public:

  TestSynth() : carrierAmt(1.0f) {
    
    carrier.freq(1000);
    modulator.freq(400);
      
    outputGen =
    (
      ( modulator + carrierAmt ) * carrier
    ) * 0.1f;
    
  };
  
  inline void setCarrierAmt(TonicFloat newCarrierAmt) { carrierAmt = clamp(newCarrierAmt, 0.0f, 1.0f); };
  inline void setCarrierFreq(TonicFloat nCarFreq) { carrier.freq(nCarFreq); };
  inline void setModFreq(TonicFloat nModFreq) { modulator.freq(nModFreq); };
  
private:
  
  SineWave carrier;
  SineWave modulator;
  
  TonicFloat carrierAmt;
  
};

#endif
