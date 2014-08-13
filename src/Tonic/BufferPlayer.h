//
//  BufferPlayer.h
//  Tonic 
//
//  Created by Morgan Packard on 10/26/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//



#ifndef TONIC_BUFFERPLAYER_H
#define TONIC_BUFFERPLAYER_H

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
    ControlGenerator trigger_;
    ControlGenerator startPosition_;
    bool isFinished_;
    
    void copySamplesToOutputBuffer(int startSample, int numSamples);
    
    public:
      BufferPlayer_();
      ~BufferPlayer_();
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
      void setBuffer(SampleTable sampleTable);
      void setDoesLoop(ControlGenerator doesLoop){doesLoop_ = doesLoop;}
      void setTrigger(ControlGenerator trigger){trigger_ = trigger;}
      void setStartPosition(ControlGenerator startPosition){startPosition_ = startPosition;}
      
    };
    
    inline void BufferPlayer_::copySamplesToOutputBuffer(int startSample, int numSamples){
      memcpy(&outputFrames_[0], &buffer_.dataPointer()[startSample], numSamples * sizeof(TonicFloat));
    }
    
    
  }
  
  /*!
    Simply plays back a buffer. "loop" parameter works, but doesn't wrap between ticks, so mostly likely you'll wind up with a few zeroes at the end of 
    the last buffer if you're looping. In other words, buffer lenghts are rounded up to the nearest kSynthesisBlockSize 
   
    Usage:
    
    SampleTable buffer = loadAudioFile("/Users/morganpackard/Desktop/trashme/2013.6.5.mp3");
    bPlayer.setBuffer(buffer).loop(false).trigger(ControlMetro().bpm(100));
   
  */
  
  class BufferPlayer : public TemplatedGenerator<Tonic_::BufferPlayer_>{
    
  public:
  
    BufferPlayer& setBuffer(SampleTable buffer){
      gen()->setBuffer(buffer);
      return *this;
    };
    
    TONIC_MAKE_CTRL_GEN_SETTERS(BufferPlayer, loop, setDoesLoop)
    TONIC_MAKE_CTRL_GEN_SETTERS(BufferPlayer, trigger, setTrigger)
    TONIC_MAKE_CTRL_GEN_SETTERS(BufferPlayer, startPosition, setStartPosition)

  };
}

#endif


