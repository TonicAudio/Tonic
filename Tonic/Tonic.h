//
//  Tonic.h
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/23/13.
//
//

#ifndef _013_1_23_melody_Tonic_h
#define _013_1_23_melody_Tonic_h

#include <stdio.h>
#include <string>
#include <math.h>


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
