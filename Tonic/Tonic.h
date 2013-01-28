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
