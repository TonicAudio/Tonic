//
//  Mixer.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/9/13.

//
// See LICENSE.txt for license and usage information.
//


//! A mixer is like an adder but acts as a source and allows dynamic removal

#ifndef __Tonic__Mixer__
#define __Tonic__Mixer__

#include "Synth.h"
#include "CompressorLimiter.h"

using std::vector;

namespace Tonic {

  class Mixer : public BufferFiller{
  
  private:
    
    TonicFrames workSpace_;
    vector<BufferFiller*> inputs_;    
    TONIC_MUTEX_T input_mutex_;
        
  public:
        
    Mixer();
    ~Mixer();
    
    void addInput(BufferFiller* input);
    void removeInput(BufferFiller* input);
        
    void tick( TonicFrames& frames, const Tonic_::SynthesisContext_ & context );
  };
}

#endif /* defined(__Tonic__Mixer__) */


