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
      
    public:
      BufferPlayer_();
      ~BufferPlayer_();
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
      void setBuffer(SampleTable sampleTable);
      
    };
    
    inline void BufferPlayer_::computeSynthesisBlock(const SynthesisContext_ &context){
      int samplesLeftInBuf = buffer_.size() - currentSample;
      int samplesToCopy = min(kSynthesisBlockSize, samplesLeftInBuf);
      memcpy(&outputFrames_[0], &buffer_.dataPointer()[currentSample], samplesToCopy * sizeof(TonicFloat));
      if (samplesToCopy < kSynthesisBlockSize) {
        currentSample = 0;
      }else{
        currentSample += kSynthesisBlockSize;
      }
    }
    
  }
  
  class BufferPlayer : public TemplatedGenerator<Tonic_::BufferPlayer_>{
    
  public:
  
    void setBuffer(SampleTable buffer){
      gen()->setBuffer(buffer);
    };

  };
}

#endif /* defined(__Tonic__BufferPlayer__) */


