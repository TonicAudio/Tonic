//
//  FilterTest.h
//  TonicDemo
//
//  Created by Nick Donaldson on 2/7/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//



#ifndef TonicDemo_FilterTest_h
#define TonicDemo_FilterTest_h

#include "Synth.h"
#include "Noise.h"
#include "TestFilt.h"

using namespace Tonic;

class FilterTest : public Synth {
  
  
public:
  FilterTest(){
    outputGen = TestFilt().input( Noise() ) * 0.5;
  }
  
  static SourceRegister<FilterTest> reg;
};


SourceRegister<FilterTest> FilterTest::reg("FilterTest");

#endif
