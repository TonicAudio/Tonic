//
//  BRCA1.cpp
//  a genetic 'sequencer'
//  TonicDemo
//
//  Created by David Wang with help from Morgan Packard on 17 May 2013.
//  2013
//

#ifndef TonicDemo_XYSpeed_cpp
#define TonicDemo_XYSpeed_cpp

#include "Tonic.h"
#include "string.h"

using namespace Tonic;

class BRCA1 : public Synth{
  
public:
  BRCA1(){
    // DNA sequence taken from http://www.ncbi.nlm.nih.gov/gene/672
    string DNAsequence = "GTACCTTGATTTCGTATTCTGAGAGGCTGCTGCTTAGCGGTAGCCCCTTGGTTTCCGTGGCAACGGAAAAGCGCGGGAATTACAGATAAATTAAAACTGCGACTGCGCGGCGTGAGCTCGCTGAGACTTCCTGGACGGGGGACAGGCTGTGGGGTTTCTCAGATAACTGGGCCCCTGCGCTCAGGAGGCCTTCACCCTCTGCTCTGGGTAAAGGTAGTAGAGTCCCGGGAAAGGGACAGGGGGCCCAAGTGATGCTCTGGGGTACTGGCGTGGGAGAGTGGATTTCCGAAGCTGACAGATGGGTATTCTTTGACGGGGGGTAGGGGCGGAACCTGAGAGGCGTAAGGCGT";
    const int NUM_STEPS = DNAsequence.length();
    char DNAsequenceArray[NUM_STEPS + 1] ;
    strcpy(DNAsequenceArray, DNAsequence.c_str());
    
    float bpm = 90;
    ControlGenerator metronome = ControlMetro().bpm(bpm);
    
    ControlSwitcher notes = ControlSwitcher().inputIndex(ControlStepper().end(NUM_STEPS).trigger(metronome));
    
    srand(time(NULL)) ;
    int index = 0;
    for(int i = 0; i < NUM_STEPS; i++){
      char nucleotide = DNAsequenceArray[index]; // i = index for now
      int octaveMultiplier = (rand() % 4) + 1; // random number of 1, 2, 3, or 4 (dividing by 3 isn't an octave but it's ok)
      switch(nucleotide) {
        case 'G':
          notes.addInput(391.995 / octaveMultiplier);
          break;
        case 'A':
          notes.addInput(440 / octaveMultiplier);
          break;
        case 'T':
                                                        // add a rest here
          //notes.addInput(293.665 / octaveMultiplier); // D
          //notes.addInput(311.127 / octaveMultiplier); // E flat
          //notes.addInput(329.628 / octaveMultiplier); // E
          notes.addInput(349.228 / octaveMultiplier); // F
          //notes.addInput(369.994 / octaveMultiplier); // F sharp
          //notes.addInput(466.164 / octaveMultiplier); // B flat
          break;
        case 'C':
          notes.addInput(261.626 / octaveMultiplier);
          break;
      } // end switch
      index++;

    } // end for
    
    
    BasicDelay delay = BasicDelay(0.5f, 1.0f)
    .delayTime( 60.0/bpm )
    .feedback( 0.0f )
    .wetLevel( 1.0f );
    
    BasicDelay delay2 = BasicDelay(0.5f, 2.0f)
    .delayTime( 2*60.0/bpm )
    .feedback( 0.0f )
    .wetLevel( 1.0f );
    
    BasicDelay delay3 = BasicDelay(0.5f, 2.0f)
    .delayTime( 3*60.0/bpm )
    .feedback( 0.0f )
    .wetLevel( 1.0f );
    
    BasicDelay delayFunky = BasicDelay(0.5f, 1.0f)
    .delayTime( 1.0f )
    .feedback( 0.0f )
    .wetLevel( 1.0f );
    
    Generator oscTest = SineWave().freq(notes) ;
    
    //outputGen = oscTest * 0.25;
    //outputGen = ( (oscTest >> delay >> delayFunky) ) * 0.10;
    outputGen = ( (oscTest >> delay >> delay2 >> delay3) ) * 0.10;
    //outputGen = ( (oscTest >> delay) + (oscTest >> delay2) + (oscTest >> delay3) ) * 0.10;
    
  }
  
}; // end synth


registerSynth(BRCA1);

#endif
