//
//  RectWave.h
//  Tonic 
//
//  Created by Nick Donaldson on 3/2/13.
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

#ifndef __Tonic__RectWave__
#define __Tonic__RectWave__

#include "Generator.h"
#include "FixedValue.h"

namespace Tonic {
  
  namespace Tonic_ {

    class RectWave_ : public Generator_
    {
      
      protected:  
        Generator freqGen_;
        Generator pwmGen_;
        
        TonicFrames freqFrames_;
        TonicFrames pwmFrames_;
        
        float phaseAccum_;
        
      public:
        RectWave_();
        ~RectWave_();
        
        void setFrequencyGenerator( Generator gen ){
          freqGen_ = gen;
        }
        
        void setPwmGenerator( Generator gen ){
          pwmGen_ = gen;
        }
        
        void computeSynthesisBlock( const SynthesisContext_ &context );
        
    };
      
    inline void RectWave_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      // tick freq and pwm
      lockMutex();
      freqGen_.tick(freqFrames_, context);
      pwmGen_.tick(pwmFrames_, context);
      unlockMutex();
      
      // somewhat inefficient implementation, perhaps...
      
      static TonicFloat const T = 1.0f/Tonic::sampleRate();
      
      TonicFloat period;
      TonicFloat *outptr = &synthesisBlock_[0];
      TonicFloat *freqptr = &freqFrames_[0];
      TonicFloat *pwmptr = &pwmFrames_[0];
      
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){

        // update the current period in seconds
        period = 1.0f/(*freqptr++);
        
        // update the current phase in seconds
        phaseAccum_ += T;
        
        // wrap the phase
        phaseAccum_ = fmodf(phaseAccum_, period);
        
        // compute output sample
        *outptr++ = phaseAccum_ > (period * clamp(*pwmptr++, 0.0f, 1.0f)) ? -1.0f : 1.0f;
      }
      
        
    }
    
  }
  
  //! Quick-and-dirty rectangular wave.
  /*! 
      WARNING: Is NOT anti-aliased!! Best for use as LFO, or if you just don't care :)
  */
  class RectWave : public TemplatedGenerator<Tonic_::RectWave_>{
    
  public:
    
    createGeneratorSetters(RectWave, freq, setFrequencyGenerator);
    createGeneratorSetters(RectWave, pwm, setPwmGenerator);

  };
}

#endif /* defined(__Tonic__RectWave__) */


