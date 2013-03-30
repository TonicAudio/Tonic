//
//  BufferFiller.cpp
//  TonicDemo
//
//  Created by Nick Donaldson on 2/9/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
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

#include "BufferFiller.h"

namespace Tonic {
  
  BufferFiller::BufferFiller() : bufferReadPosition_(0){
    outputFrames_.resize(kSynthesisBlockSize, 2);
    outputGen = PassThroughGenerator();
  }
  
}