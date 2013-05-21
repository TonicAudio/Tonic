
#include "Tonic.h"

using namespace Tonic;

// Expects a RingBuffer called "input" to already exist and be capturing input
class InputDemoSynth : public Synth {
  
public:
  
  InputDemoSynth(){
    
    RingBufferReader inputReader = RingBufferReader().bufferName("input");
    
    Generator ringModInput = inputReader * SineWave().freq(250);
    
    BasicDelay delay = BasicDelay(1.2f).delayTime(0.4f).wetLevel(0.4f).dryLevel(1.0f).feedback(0.4f);
    
    setOutputGen((ringModInput >> delay) * 0.5f);
  }
  
};

registerSynth(InputDemoSynth);