//
//  Mixer.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/9/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//


//! A mixer is like an adder but acts as a source and allows dynamic removal

#ifndef __Tonic__Mixer__
#define __Tonic__Mixer__

#include "BufferFiller.h"
#include "CompressorLimiter.h"

using std::vector;

namespace Tonic {

  class Mixer : public BufferFiller{
  
  private:
    TonicFrames workSpace_;
    vector<BufferFiller*> inputs_;
    Limiter limiter_;
    
    bool limitOutput_;
    
  public:
    
    Mixer();
    
    void addInput(BufferFiller* input);
    void removeInput(BufferFiller* input);
    
    void setLimitOutput(bool shouldLimit) { limitOutput_ = shouldLimit; };
    
    void tick( TonicFrames& frames, const Tonic_::SynthesisContext_ & context );
  };
}

#endif /* defined(__Tonic__Mixer__) */


