//
//  CompressorTestSynth.cpp
//  TonicDemo
//
//  Created by Nick Donaldson on 3/25/13.

//


#include "Tonic.h"

using namespace Tonic;

class CompressorTestSynth : public Synth
{
  
public:
  CompressorTestSynth(){
    
    ControlValue threshold = addParameter("threshold", "Threshold (dbFS)", -12.f, -60.f, 0.f);
    ControlValue ratio = addParameter("ratio", "Ratio", 2.0, 1.0, 64.f, true); //logarithmic
    ControlValue attack = addParameter("attackTime", "Attack Time (s)", 0.001, 0.001, 0.1, true); //logarithmic
    
    // Emulation of 808 snare with long, quiet noise decay
    Generator hpNoise = (Noise() * dBToLin(-18.0)) >> HPF24().cutoff(2000.0) >> LPF12().cutoff(8000);
    Generator tones = SineWave().freq(180) * dBToLin(-6.0) + SineWave().freq(332) * dBToLin(-18.0);

    ControlGenerator snareGate = ControlMetro(80) >> ControlPulse(60.0*0.8/80.0);
    ADSR toneADSR = ADSR(0.0005, 0.03, 0.0, 0.01).trigger(snareGate);
    ADSR noiseADSR = ADSR(0.001, 0.25, 0.0, 0.25).trigger(snareGate);
  
    // Squaring gives something a bit closer to exponential
    // TODO: Exponential ADSR
    Generator noiseEnv = noiseADSR * noiseADSR;
    
    Compressor compressor = Compressor()
                            .attack(attack)
                            .release(0.06)
                            .threshold( addParameter("compThresh", 1.0) )
                            .ratio(8)
                            .lookahead(0.001);
        
    outputGen = ( ( ( tones * toneADSR ) + ( hpNoise * noiseEnv ) ) >> compressor ) * addParameter("compGain", 1.0).smoothed() * 0.5;
  }
  
};

registerSynth(CompressorTestSynth);