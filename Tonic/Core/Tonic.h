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
  #define ARC4RAND_MAX 0x100000000
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
  
  // -- Global Constants --
  
  static TonicFloat sampleRate(){
      return 44100;
  };

  static const unsigned int kSynthesisBlockSize = 64;
  
  namespace Tonic_{
    // -- Synthesis Context Struct --
    
    // Context passed down from root BufferFiller graph object to all sub-generators
    // Synchronizes signal flow in cases when generator output is shared between multiple inputs
    struct SynthesisContext_{
      
      // Number of frames elapsed since program start
      // unsigned long will last 38+ days before overflow at 44.1 kHz
      unsigned long elapsedFrames;
      
      // System time in seconds
      double elapsedTime;
      
      SynthesisContext_() : elapsedFrames(0), elapsedTime(0) {};
      
      void tick() {
        elapsedFrames += kSynthesisBlockSize;
        elapsedTime += (double)kSynthesisBlockSize/sampleRate();
      };
    
    };
    
  } // namespace Tonic_

#pragma mark - Utility Functions
  
  //-- Vector Math --
  inline static void vcopy( TonicFloat * dst, unsigned int dst_str, const TonicFloat * src, unsigned int src_str, unsigned int length ){
    for (unsigned int i=0; i<length; i++, dst += dst_str, src += src_str){
      *dst = *src;
    }
  }
  
  //-- Arithmetic --
  
  inline static TonicFloat max(TonicFloat a, TonicFloat b) {
    return (a > b ? a : b);
  }

  inline static TonicFloat min(TonicFloat a, TonicFloat b) {
    return (a < b ? a : b);
  }

  inline static TonicFloat clamp(TonicFloat val, TonicFloat min, TonicFloat max) {
    return (val < min ? min : (val > max ? max : val));
  }
  
  inline static TonicFloat map(TonicFloat val, TonicFloat inMin, TonicFloat inMax, TonicFloat outMin, TonicFloat outMax, bool clamp = true) {
    TonicFloat result = ((val - inMin)/(inMax - inMin)) * (outMax - outMin) + outMin;
    if (clamp){
      result = Tonic::clamp(result, min(outMin,outMax), max(outMin,outMax));
    }
    return result;
  }
  
  //-- Freq/MIDI --
  
  //! Midi note number to frequency in Hz
  inline static TonicFloat mtof(TonicFloat nn){
    return 440.0f * powf(2.0, (nn-69.0f)/12.0f);
  }
  
  //! Frequency in Hz to midi note number
  inline static TonicFloat ftom(TonicFloat f){
    return 12.0f * log2(f/440.0f) + 69.0f;
  }
  
  //-- Decibels --
  /*
    Working with 0 dB representing 1.0
  */
  inline static TonicFloat linTodB(TonicFloat lv){
    return 20.0f*log(max(0, lv));
  }
  
  inline static TonicFloat dBToLin(TonicFloat dBv){
    return exp(dBv/20.0f);
  }
  
  // -- Misc --
  inline static TonicFloat randomSample(){
    #ifdef __APPLE__
    return ((TonicFloat)arc4random()/ARC4RAND_MAX) * 2.0f - 1.0f;
    #else
    return ((TonicFloat)rand()/RAND_MAX) * 2.0f - 1.0f;
    #endif
  }
  
  // -- Logging --
  
  static void error(std::string message){
    printf("Tonic::error: %s\n",  message.c_str());
  }
  
  static void debug(std::string message){
    printf("Tonic::debug: %s\n", message.c_str());
  }
  
}


#endif
