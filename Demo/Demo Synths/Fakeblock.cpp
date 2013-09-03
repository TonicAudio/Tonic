//
//  Fakeblock.cpp
//  a Boolean driven aggregation of what programmers call 'hacker traps'
//  heavily modified from http://www.boyinaband.com/tutorials/electro-pop-tutorial-–-part-9-a-pitched-wood-block-style-synth/ but it didn't sound accurate
//  TonicDemo
//  of course, thanks to Arrested Development for the idea
//
//  Created by David Wang on 25 July 2013.
//  2013
//

#ifndef TonicDemo_XYSpeed_cpp
#define TonicDemo_XYSpeed_cpp

#include "Tonic.h"
#include "string.h"

using namespace Tonic;

class Fakeblock : public Synth{
  
public:
  Fakeblock(){

    float bpm = 30;
    ControlGenerator metronome = ControlMetro().bpm(bpm);
    
    float frequency = 523.2 ; // C
    float volume = 0.2 ;
    Generator oscSine = SineWave().freq(frequency) * volume ;
    Generator oscRect = RectWave().freq(frequency).pwm( 0.5 ) * volume/2;
    Generator oscTri = TriangleWave().freq(frequency) * volume ;
    Generator oscSaw = SawtoothWave().freq(frequency / 2) * volume ;
    Generator oscNoise = Noise() * volume / 2;
    
    ADSR envelopeRect = ADSR()
    .attack(0.01)
    .decay( 0 )
    .sustain(0)
    .release(0.1)
    .doesSustain(false)
    .legato(true)
    .trigger(metronome);
    
    ADSR envelopeTri = ADSR()
    .attack(0.01)
    .decay( 0 )
    .sustain(0)
    .release(0.6)
    .doesSustain(false)
    .legato(true)
    .trigger(metronome);
    
    BasicDelay delay = BasicDelay(0.01f, 1.0f)
    .delayTime( 0.01f )
    .feedback( 0.1f )
    .wetLevel( 1.0f );
    
    Reverb reverb = Reverb()
    .preDelayTime(0)
    .inputLPFCutoff(8000)
    .inputHPFCutoff(100)
    .decayTime(2)
    .decayLPFCutoff(8000)
    .decayHPFCutoff(100)
    .stereoWidth(0.5f)
    .density(0.5f)
    .roomShape(0.5f)
    .roomSize(0.5f);

    outputGen = (oscRect * envelopeRect + oscTri * envelopeTri) >> reverb >> BPF12().cutoff(frequency) >> LPF12().cutoff(8000);
    //outputGen = (toneHarmonic + oscTri) * envelope >> BPF12().cutoff(frequency) >> LPF12().cutoff(8000);


  }
};

registerSynth(Fakeblock);

#endif