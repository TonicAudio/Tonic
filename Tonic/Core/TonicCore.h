//
//  TonicCore.h
//  Tonic
//
//  Created by Morgan Packard on 1/23/13.
//
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__TonicCore__
#define __Tonic__TonicCore__

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <stdio.h>
#include <math.h>
#include <pthread.h>

#ifdef __APPLE__
  #import <Accelerate/Accelerate.h>
  #define USE_APPLE_ACCELERATE_disabled
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

// Causes 32nd bit in double to have fractional value 1 (decimal point on 32-bit word boundary)
// Allowing some efficient shortcuts for table lookup using power-of-two tables
#define BIT32DECPT 1572864.0

#define TONIC_DEBUG

namespace Tonic {
  
  // -- Global Constants --
  
  static TonicFloat sampleRate(){
      return 44100;
  };

  static const unsigned int kSynthesisBlockSize = 64;
  
  
  // -- Global Types --
  
  // For fast computation of int/fract using some bit-twiddlery
  // inspired by the pd implementation
  union ShiftedDouble {
    double d;
    uint32_t i[2];
  };
  
  
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
    return 20.0f*log10f(max(0, lv));
  }
  
  inline static TonicFloat dBToLin(TonicFloat dBv){
    return powf(10,(dBv/20.0f));
  }
  
  // -- Misc --
  inline static TonicFloat randomSample(){
    #ifdef __APPLE__
    return ((TonicFloat)arc4random()/ARC4RAND_MAX) * 2.0f - 1.0f;
    #else
    return ((TonicFloat)rand()/RAND_MAX) * 2.0f - 1.0f;
    #endif
  }
  
  static float randomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

  //! Tonic exception class
  class TonicException : public runtime_error
  {
    public:
    TonicException(string const& error) : runtime_error(error) {};

    // May want to implement custom exception behavior here, but for now, this is essentially a typedef

  };
  
  
  // -- Logging --
  
  static void error(string message, bool fatal = false){
    // maybe also log to console?
    printf("Tonic::error: %s\n", message.c_str() );
    if (fatal){
      throw TonicException(message);
    }
  }
  
  static void warning(string message){
    printf("Tonic::warning: %s\n", message.c_str());
  }
  
  static void debug(std::string message){
    printf("Tonic::debug: %s\n", message.c_str());
  }


  
} // namespace Tonic


#endif
