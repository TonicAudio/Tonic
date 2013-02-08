//
//  Tonic.h
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/23/13.
//
//

/*+++++++++++++++++++++ License ++++++++++++++++++++

Use this code for whatever you want. There are NO 
RESTRICTIONS WHATSOVER. Modify it, repackage it, 
sell it, get rich from it, whatever. Go crazy. If 
you want to make mehappy, contribute to this 
project, but feel free to just use the code as a 
starting point for whatever you like.

Note that Tonic is heavily indebted to STK
https://ccrma.stanford.edu/software/stk/

++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef _013_1_23_melody_Tonic_h
#define _013_1_23_melody_Tonic_h

#include <stdio.h>
#include <string>
#include <math.h>
#include <pthread.h>

#ifdef __APPLE__
  #import <Accelerate/Accelerate.h>
  #define USE_APPLE_ACCELERATE
#endif

using namespace std;

typedef unsigned long         UInt32;
typedef float                 TonicFloat;

#ifndef PI
const TonicFloat PI           = 3.14159265358979;
#endif

#ifndef TWO_PI
const TonicFloat TWO_PI       = 2 * PI;
#endif


namespace Tonic {

  // A few helper functions. May or may not belong here...
  
  //-- Arithmetic --
  
  static TonicFloat max(TonicFloat a, TonicFloat b) {
    return (a > b ? a : b);
  };

  static TonicFloat min(TonicFloat a, TonicFloat b) {
    return (a < b ? a : b);
  }

  static TonicFloat clamp(TonicFloat val, TonicFloat min, TonicFloat max) {
    return (val < min ? min : (val > max ? max : val));
  }
  
  static TonicFloat map(TonicFloat val, TonicFloat inMin, TonicFloat inMax, TonicFloat outMin, TonicFloat outMax, bool clamp = true) {
    TonicFloat result = ((val - inMin)/(inMax - inMin)) * (outMax - outMin) + outMin;
    if (clamp){
      result = Tonic::clamp(result, outMin, outMax);
    }
    return result;
  }
  
  //-- Freq/MIDI --
  
  static TonicFloat mtof(TonicFloat nn){
    return 440.0f * powf(2.0, (nn-69.0f)/12.0f);
  }
  
  static TonicFloat ftom(TonicFloat f){
    return 12.0f * log2(f/440.0f) + 69.0f;
  }
  
  //-- Decibels --
  /*
    Working with 0 dB representing 1.0
  */
  static TonicFloat linTodB(TonicFloat lv){
    return 20.0f*log(max(0, lv));
  }
  
  static TonicFloat dBToLin(TonicFloat dBv){
    return exp(dBv/20.0f);
  }
  
  // --
    
  static TonicFloat sampleRate(){
    return 44100;
  };
  
  static const int            kSynthesisBlockSize = 64;
  
  static void error(std::string message){
    printf("Tonic::error: %s\n",  message.c_str());
    exit(0);
  }
  
}


#endif
