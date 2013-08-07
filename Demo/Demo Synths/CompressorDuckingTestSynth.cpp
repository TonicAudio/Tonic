//
//  CompressorDuckingTestSynth.cpp
//  TonicDemo
//
//  Created by Nick Donaldson on 3/26/13.

//

#include "Tonic.h"

using namespace Tonic;

class CompressorDuckingTestSynth : public Synth
{
  
public:
  CompressorDuckingTestSynth(){
    
    // Emulation of 808 snare with long, quiet noise decay
    Generator hpNoise = (Noise() * dBToLin(-18.0)) >> HPF24().cutoff(2000.0) >> LPF12().cutoff(8000);
    Generator tones = SineWave().freq(180) * dBToLin(-6.0) + SineWave().freq(332) * dBToLin(-18.0);
    
    ControlMetro metro = ControlMetro(120);
    ControlGenerator snareGate = ControlPulse(60.0*0.8/120.0).trigger(metro);
    ADSR toneADSR = ADSR(0.0005, 0.03, 0.0, 0.01).trigger(snareGate);
    ADSR noiseADSR = ADSR(0.001, 0.25, 0.0, 0.25).trigger(snareGate);
    
    // Squaring gives something a bit closer to exponential
    // TODO: Exponential ADSR
    Generator noiseEnv = noiseADSR * noiseADSR;
    
    // duck the crap out of it
    Compressor duckingComp = Compressor()
    .attack(0.001)
    .release( addParameter("compRelease", 0.025) )
    .threshold( dBToLin(-52) )
    .ratio(16)
    .lookahead(0.001);
    
    Generator snare =  ( tones * toneADSR ) + ( hpNoise * noiseEnv );
    
    float bassScaleArr[4] = {0,7,10,12};
    vector<float> bassScale(bassScaleArr, bassScaleArr + sizeof bassScaleArr / sizeof bassScaleArr[0]);
    
    Generator baseFreq = ( (30 + (ControlRandom().min(0).max(12).trigger(metro) >> ControlSnapToScale().setScale(bassScale)) ) >> ControlMidiToFreq()).smoothed();
    
    ADSR bassFiltADSR = ADSR(0.005, 0.1, 0, 0.05).doesSustain(false).legato(true).trigger(metro);
    
    Generator randomBass = (RectWave().freq( baseFreq ) * 0.5) >> LPF24().cutoff( 300 * (1 + bassFiltADSR * 2) * (1 + ((SineWave().freq(0.1) + 1) * 0.5))).Q(1.5);
    
    StereoDelay delay = StereoDelay(0.37, 0.38).feedback(0.4).dryLevel(0.8).wetLevel(0.2);
    
    setOutputGen(duckingComp.audioInput(randomBass >> delay).sidechainInput(snare)  + snare * 0.5);
  }
  
};

TONIC_REGISTER_SYNTH(CompressorDuckingTestSynth);