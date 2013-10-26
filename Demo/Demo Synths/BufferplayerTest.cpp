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
    
    SampleTable buffer(3 * 44100, 1);
    
    float freq = 300;
    float TWO_PI = 2 * PI;
    float angleInc = freq * TWO_PI / 44100;
    float angle = 0;
    for (int i = 0; i < buffer.size(); i++) {
      *(buffer.dataPointer() + i) = sin(angle);
      angle += angleInc;
    }
    
    bPlayer.setBuffer(buffer);
    
    setOutputGen(bPlayer);
  }

};


TONIC_REGISTER_SYNTH(BufferPlayerTest);

#endif
