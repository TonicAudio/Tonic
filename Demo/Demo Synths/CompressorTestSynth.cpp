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
    
    ControlParameter threshold = addParameter("threshold", -12.f).displayName("Threshold (dbFS)").min(-60.f).max(0.f);
    ControlParameter ratio = addParameter("ratio", 2.0f).displayName("Ratio").min(1.0f).max(64.f).logarithmic(true);
    ControlParameter attack = addParameter("attackTime", 0.001).displayName("Attack Time (s)").min(0.001).max(0.1).logarithmic(true);
    ControlParameter release = addParameter("releaseTime", 0.05).displayName("Release Time (s)").min(0.01).max(0.08).logarithmic(true);
    ControlParameter gain = addParameter("gain", 0.f).displayName( "Makeup Gain (dbFS)").min(0.f).max(36.0f);
    ControlParameter bypass = addParameter("bypass",false).parameterType(ControlParameterTypeToggle);
    
    // Emulation of 808 snare with long, quiet noise decay
    Generator hpNoise = (Noise() * dBToLin(-18.0)) >> HPF24().cutoff(2000.0) >> LPF12().cutoff(8000);
    Generator tones = SineWave().freq(180) * dBToLin(-6.0) + SineWave().freq(332) * dBToLin(-18.0);

    ControlGenerator snareGate = ControlPulse(60.0*0.8/80.0).trigger( ControlMetro(80) );
    ADSR toneADSR = ADSR(0.0005, 0.03, 0.0, 0.01).trigger(snareGate);
    ADSR noiseADSR = ADSR(0.001, 0.25, 0.0, 0.25).trigger(snareGate);
  
    // Squaring gives something a bit closer to exponential
    // TODO: Exponential ADSR
    Generator noiseEnv = noiseADSR * noiseADSR;
    
    Compressor compressor = Compressor()
                            .attack(attack)
                            .release(0.06)
                            .threshold( ControlDbToLinear().input(threshold) )
                            .ratio( ratio )
                            .lookahead(0.001)
                            .makeupGain(ControlDbToLinear().input(gain))
                            .bypass(bypass);
        
    Generator outputGen = ( ( ( tones * toneADSR ) + ( hpNoise * noiseEnv ) ) >> compressor ) * 0.5;
    
    setOutputGen(outputGen);
  }
  
};

TONIC_REGISTER_SYNTH(CompressorTestSynth);