//
//  BufferplayerTest.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 10/26/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_BufferplayerTest_cpp
#define TonicDemo_BufferplayerTest_cpp

#include "Tonic.h"

using namespace Tonic;

class BufferPlayerTest : public Synth{

  public:
  BufferPlayerTest(){


    BufferPlayer bPlayer;
    
    SampleTable buffer(6 * 44100, 2);

    // fill the buffer with a sine wave
//    float freq = 300;
//    float TWO_PI = 2 * PI;
//    float angleInc = freq * TWO_PI / 44100;
//    float angle = 0;
//    for (int i = 0; i < buffer.frames(); i++) {
//      for(int channel = 0; channel < buffer.channels(); channel++){
//        *(buffer.dataPointer() + i * buffer.channels() + channel) = sin(angle);
//      }
//      angle = fmod(angle + angleInc, TWO_PI);
//    }
    
    buffer = loadAudioFile("/Users/morganpackard/Desktop/trashme/2013.6.5.mp3");
//
    bPlayer.setBuffer(buffer);
    
    setOutputGen(bPlayer);
  }

};


TONIC_REGISTER_SYNTH(BufferPlayerTest);

#endif
