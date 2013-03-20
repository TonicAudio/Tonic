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

#ifndef __Tonic__BufferFiller__
#define __Tonic__BufferFiller__

#include <map>
#include "Generator.h"

//! BufferFiller is the base class for any generator expected to produce output for a buffer fill.
/*!
 BufferFillers provide a high-level interface for combinations of generators, and can be used to fill
 arbitraryly large buffers.
  
 Subclasses to include mixer, channel, synth, etc.
*/
namespace Tonic{
  
  class BufferFiller : public Generator {
    
  private:
    int                         bufferReadPosition_;
    TonicFrames                 outputFrames_;
    Tonic_::SynthesisContext_   context_;
    
  protected:
    
    Generator     outputGen;
    
  public:
    BufferFiller();
    
    //! tick method without context argument passes down this instance's SynthesisContext_
    void tick( TonicFrames& frames );
    
    //! tick method with context argument uses passed-in context
    virtual void tick( TonicFrames& frames, const Tonic_::SynthesisContext_ & context );
    
    //! Fill an arbitrarily-sized, interleaved buffer of audio samples as floats
    /*!
        This BufferFiller's outputGen is used to fill an interleaved buffer starting at outData.
    */
    void fillBufferOfFloats(float *outData,  unsigned int numFrames, unsigned int numChannels);
    
    //! Returns a reference to outputGen
    inline const Generator & getOutputGenerator() { return outputGen; };
  };
  
}

#endif /* defined(__TonicDemo__BufferFiller__) */
