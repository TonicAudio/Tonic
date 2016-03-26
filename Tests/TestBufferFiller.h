//
//  TestBufferFiller.h
//  TonicDemo
//
//  Created by Nick D on 4/21/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef __TonicDemo__TestBufferFiller__
#define __TonicDemo__TestBufferFiller__

#include "Tonic.h"

using namespace Tonic;

// BufferFiller with publicly settable output generator for testing

class TestBufferFiller : public Synth
{
  
public:
  
  TestBufferFiller(){
      setLimitOutput(false);
  }
  
};

#endif /* defined(__TonicDemo__TestBufferFiller__) */
