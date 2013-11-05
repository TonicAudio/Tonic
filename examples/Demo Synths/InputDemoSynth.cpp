
#include "Tonic.h"

using namespace Tonic;

// Expects a RingBuffer called "input" to already exist and be capturing input
class InputDemoSynth : public Synth {
  
public:
  
  InputDemoSynth(){
    
    ControlParameter inputGain = addParameter("inputGain", 0.f).displayName("Input Gain (dbFS)").min(-12.f).max(12.f);
    ControlParameter ringFreq = addParameter("ringFreq", 10.f).displayName("Ring Mod Frequency").min(10.f).max(1000.f).logarithmic(true);
    
    // This will try to read from a globally-known ring buffer called "input"
    RingBufferReader inputReader = RingBufferReader().bufferName("input");
    
    Generator ringModInput = inputReader * (inputGain >> ControlDbToLinear()).smoothed() * SineWave().freq( ringFreq.smoothed(0.5) );
    
    BasicDelay delay = BasicDelay(1.2f).delayTime(0.5f).wetLevel(0.4f).dryLevel(1.0f).feedback(0.6f);
    
    setOutputGen((ringModInput >> delay) * 0.5f);
  }
  
};

TONIC_REGISTER_SYNTH(InputDemoSynth);