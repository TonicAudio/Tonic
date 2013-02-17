//
//  SineAMSynth,h
//  Tonic
//
//  Created by Nick Donaldson on 02/07/2013
//
//

#ifndef _013_1_23_melody_TestSynth_h
#define _013_1_23_melody_TestSynth_h

#include "Tonic.h"
#include "Synth.h"
#include "SineWave.h"
#include "Multiplier.h"
#include "Adder.h"
#include "FixedValue.h"
#include "RampedValue.h"

using namespace Tonic;

class SineAMSynth : public Synth{

public:

  SineAMSynth() : carrierAmt(1.0f) {
    
    // TODO: do this by registering messages
    carrierFreq.value(400).lengthMs(2000);
    modFreq.value(10).lengthMs(200);
    
    carrier.freq(carrierFreq);
    modulator.freq(modFreq);
      
    outputGen =
    (
      ( modulator + carrierAmt ) * carrier
    ) * 0.5f;
    
  };
  
  inline void setCarrierAmt(TonicFloat newCarrierAmt) { carrierAmt = clamp(newCarrierAmt, 0.0f, 1.0f); };
  inline void setCarrierFreq(TonicFloat nCarFreq) { carrierFreq.target(nCarFreq); };
  inline void setModFreq(TonicFloat nModFreq) { modFreq.target(nModFreq); };
  
private:
  
  SineWave carrier;
  SineWave modulator;
  
  RampedValue carrierFreq;
  RampedValue modFreq;
  
  TonicFloat carrierAmt;
  
  static SourceRegister<SineAMSynth> reg;
  
};

SourceRegister<SineAMSynth> SineAMSynth::reg("SineAMSynth");

#endif
