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

  TestSynth() : depth(1.0) {
    
    carrier.freq(1000);
    modulator.freq(400);
      
    outputGen =
    (
      ( modulator + depth ) * carrier
    ) * 0.5;
    
  };
  
  inline void setDepth(float newDepth) { depth = newDepth; };
  inline void setCarrierFreq(float nCarFreq) { carrier.freq(nCarFreq); };
  inline void setModFreq(float nModFreq) { modulator.freq(nModFreq); };
  
private:
  
  SineWave carrier;
  SineWave modulator;
  
  float    depth;

};

#endif
