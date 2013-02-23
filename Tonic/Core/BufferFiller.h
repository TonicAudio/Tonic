//
//  BufferFiller.h
//  Tonic
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

#ifndef __TonicDemo__BufferFiller__
#define __TonicDemo__BufferFiller__

#include <iostream>
#include <map>
#include "Generator.h"

//! BufferFiller is the base class for any generator expected to produce output for a buffer fill.
/*!
 BufferFillers provide a high-level interface for combinations of generators, and can be used to fill
 arbitraryly large buffers.
  
 Subclasses to include mixer, channel, synth, etc.
*/
namespace Tonic{
  
  class BufferFiller : Generator {
    
  private:
    int               bufferReadPosition_;
    TonicFrames       outputFrames_;
    SynthesisContext  context_;
    
  protected:
    
    Generator     outputGen;
    
  public:
    BufferFiller();
    
    // Tick method without context argument passes down this instance's SynthesisContext
    void          tick( TonicFrames& frames );
    void          tick( TonicFrames& frames, const SynthesisContext & context );
    void          fillBufferOfFloats(float *outData,  UInt32 numFrames, int numChannels);
    
    inline const Generator & getOutputGenerator() { return outputGen; };
  };
  
}

#endif /* defined(__TonicDemo__BufferFiller__) */
