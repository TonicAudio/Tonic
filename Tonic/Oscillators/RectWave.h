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

#define TONIC_RECT_RES 4096

namespace Tonic {
  
  namespace Tonic_ {

    class RectWave_ : public Generator_
    {
      
      protected:  
        Generator freqGen_;
        Generator pwmGen_;
        
        TonicFrames freqFrames_;
        TonicFrames pwmFrames_;
        
        double phaseAccum_;
        
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
      freqGen_.tick(freqFrames_, context);
      pwmGen_.tick(pwmFrames_, context);
      
      const TonicFloat rateConstant =  TONIC_RECT_RES / Tonic::sampleRate();

      TonicFloat *outptr = &synthesisBlock_[0];
      TonicFloat *freqptr = &freqFrames_[0];
      TonicFloat *pwmptr = &pwmFrames_[0];
      
      ShiftedDouble sd;
      
      // pre-multiply rate constant for speed
#ifdef USE_APPLE_ACCELERATE
      vDSP_vsmul(freqptr, 1, &rateConstant, freqptr, 1, kSynthesisBlockSize);
#else
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
        *freqptr++ *= rateConstant;
      }
      freqptr = &freqFrames_[0];
#endif
      
      sd.d = BIT32DECPT;
      int32_t offs, msbi = sd.i[1];
      double ps = phaseAccum_ + BIT32DECPT;
      for ( unsigned int i=0; i<synthesisBlock_.frames(); i++ ) {
        
        sd.d = ps;
        ps += *freqptr++;
        offs = sd.i[1] & (TONIC_RECT_RES-1);
        sd.i[1] = msbi;
        
        *outptr++ = offs > (TONIC_RECT_RES * *pwmptr++) ? -1.0f : 1.0f;
      }
      
      sd.d = BIT32DECPT * TONIC_RECT_RES;
      msbi = sd.i[1];
      sd.d = ps + (BIT32DECPT * TONIC_RECT_RES - BIT32DECPT);
      sd.i[1] = msbi;
      phaseAccum_ = sd.d - BIT32DECPT * TONIC_RECT_RES;
        
    }
    
  }
  
  //! Quick-and-dirty rectangular wave.
  /*! 
      WARNING: Is NOT anti-aliased!! Best for use as LFO, or if you just don't care :)
  */
  class RectWave : public TemplatedGenerator<Tonic_::RectWave_>{
    
  public:
    
    //! Set the frequency of the waveform
    createGeneratorSetters(RectWave, freq, setFrequencyGenerator);
    
    //! Set the pulse width of the rectangle. Input should be clipped between 0-1
    createGeneratorSetters(RectWave, pwm, setPwmGenerator);

  };
}

#endif /* defined(__Tonic__RectWave__) */


