//
//  Noise.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/7/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_NOISE_H
#define TONIC_NOISE_H

#include "Generator.h"

namespace Tonic {
  
  namespace Tonic_ {
    
    class Noise_ : public Generator_{
      
    protected:
      
      void computeSynthesisBlock( const SynthesisContext_ & context );
      
    };
    
    inline void Noise_::computeSynthesisBlock( const SynthesisContext_ & context ){
      TonicFloat* fdata = &outputFrames_[0];
      for (unsigned int i=0; i<outputFrames_.size(); i++){
        *fdata++ = randomSample();
      }
    }
    
    // Pink noise generator. Sources:
    // http://www.firstpr.com.au/dsp/pink-noise/
    // http://vellocet.com/dsp/noise/VRand.html
    
    #define kNumPinkNoiseBins       16
    #define kNumPinkNoiseBinsLog2   4
    
    class PinkNoise_ : public Generator_{
      
    private:
      
      unsigned long countTrailingZeros(unsigned long n);
      
      TonicFloat    pinkAccum_;
      TonicFloat    pinkBins_[kNumPinkNoiseBins];
      unsigned long pinkCount_;
      
      void computeSynthesisBlock( const SynthesisContext_ & context);
      
    public:
      
      PinkNoise_();      

    };
    
    inline void PinkNoise_::computeSynthesisBlock( const SynthesisContext_ & context)
    {
      // TODO: stereo?
      TonicFloat binval, prevbinval;
      unsigned long binidx;
      
      TonicFloat* outptr = &outputFrames_[0];
      
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
        
        binidx = countTrailingZeros(pinkCount_);
        binidx = binidx & (kNumPinkNoiseBins-1);
        
        prevbinval = pinkBins_[binidx];
        
        while (true){
          binval = randomSample();
        
          pinkBins_[binidx] = binval;
        
          binval -= prevbinval;
          
          pinkAccum_ += binval;
          
          if (pinkAccum_ < -kNumPinkNoiseBinsLog2 || pinkAccum_ > kNumPinkNoiseBinsLog2){
            pinkAccum_ -= binval;
          }
          else break;
        }
        
        pinkCount_++;
    
        *outptr++ = (randomSample() + pinkAccum_)/(kNumPinkNoiseBinsLog2+1);
      }
  
    }
    
    inline unsigned long PinkNoise_::countTrailingZeros(unsigned long n)
    {
      unsigned long i=0;
      while (((n>>i)&1) == 0 && i<sizeof(unsigned long)*8) i++;
      return i;
    }
  }
  
  class Noise : public TemplatedGenerator<Tonic_::Noise_>{
  public:
    Noise(bool stereo = false){
      gen()->setIsStereoOutput(stereo);
    }
  };

  class PinkNoise : public TemplatedGenerator<Tonic_::PinkNoise_> {};

}

#endif


