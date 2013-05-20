
#include "Tonic.h"

using namespace Tonic;

// Expects a RingBuffer called "input" to already exist and be capturing input
class InputDemoSynth : public Synth {
  
public:
  
  InputDemoSynth(){
   
    RingBuffer inputBuffer = RingBuffer().initialize("input");
    BasicDelay delay = BasicDelay(1.2f).delayTime(1.f).wetLevel(1.f).dryLevel(0.f).feedback(0.f);
    
    setOutputGen((inputBuffer >> delay) * 0.25f);
  }
  
};

registerSynth(InputDemoSynth);