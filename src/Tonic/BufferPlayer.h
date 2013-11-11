//
//  BufferPlayer.h
//  Tonic 
//
//  Created by Morgan Packard on 10/26/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//



#ifndef __Tonic__BufferPlayer__
#define __Tonic__BufferPlayer__

#include "Generator.h"
#include "FixedValue.h"
#include "SampleTable.h"

namespace Tonic {
  
  namespace Tonic_ {

    class BufferPlayer_ : public Generator_{
      
    protected:
    
    SampleTable buffer_;
    int testVar;
    int currentSample;
    int samplesPerSynthesisBlock;
    ControlGenerator doesLoop_;
    bool isFinished_;
    
    void copySamplesToOutputBuffer(int startSample, int numSamples);
    
    public:
      BufferPlayer_();
      ~BufferPlayer_();
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
      void setBuffer(SampleTable sampleTable);
      void setDoesLoop(ControlGenerator doesLoop){doesLoop_ = doesLoop;}
      
    };
    
    inline void BufferPlayer_::copySamplesToOutputBuffer(int startSample, int numSamples){
      memcpy(&outputFrames_[0], &buffer_.dataPointer()[startSample], numSamples * sizeof(TonicFloat));
    }
    
    
  }
  
  /*!
    Simply plays back a buffer. "loop" parameter works, but doesn't wrap between ticks, so mostly likely you'll wind up with a few zeroes at the end of 
    the last buffer if you're looping. In other words, buffer lenghts are rounded up to the nearest kSynthesisBlockSize 
   
  */
  
  class BufferPlayer : public TemplatedGenerator<Tonic_::BufferPlayer_>{
    
  public:
  
    BufferPlayer& setBuffer(SampleTable buffer){
      gen()->setBuffer(buffer);
      return *this;
    };
    
    TONIC_MAKE_CTRL_GEN_SETTERS(BufferPlayer, loop, setDoesLoop)

  };
}

#endif /* defined(__Tonic__BufferPlayer__) */


