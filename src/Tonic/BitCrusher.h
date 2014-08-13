//
//  BitCrusher.h
//
//  Created by Morgan Packard on 7/17/14.
//
//

#ifndef __BitCrusher__
#define __BitCrusher__

#include <iostream>

#include "Effect.h"

#include "ControlGenerator.h"

namespace Tonic {
  
  namespace Tonic_ {
  
    class BitCrusher_ : public Effect_{
      
    protected:
      ControlGenerator bitDepth;
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    public:
      BitCrusher_();
      void setBitDepth(ControlGenerator);
      void setInput( Generator input );
      void setIsStereoInput(bool stereo);
      
    };
    
    inline void BitCrusher_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      TonicFloat *synthBlockWriteHead = &outputFrames_[0];
      TonicFloat *dryFramesReadHead = &dryFrames_[0];
      
      unsigned int nSamples = (unsigned int)outputFrames_.size();
      float bitDepthValue = clamp(bitDepth.tick(context).value, 0, 16) ;
      float bitDepthMax = pow(2, bitDepthValue);
      
      while (nSamples--){
        TonicFloat dry = *(dryFramesReadHead++);
        TonicInt32 snapped = bitDepthMax * dry;
        TonicFloat snappedFloat = (float)snapped/(float)bitDepthMax;
        *(synthBlockWriteHead++) =  snappedFloat;
      }
    }
  }
  
  class BitCrusher : public TemplatedEffect<BitCrusher, Tonic_::BitCrusher_>{
    
    public:
    
      TONIC_MAKE_CTRL_GEN_SETTERS(BitCrusher, bitDepth, setBitDepth);

  };
  
}

#endif /* defined(__RazorDust__BitCrusher__) */