//
//  RectWave.h
//  Tonic 
//
//  Created by Nick Donaldson on 3/2/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_RECTWAVE_H
#define TONIC_RECTWAVE_H

#include "Generator.h"
#include "BLEPOscillator.h"

#define TONIC_RECT_RES 4096

namespace Tonic {
  
  namespace Tonic_ {

    //! Quick-and-dirty non-bandlimited rect wave
    class RectWave_ : public Generator_
    {
      
      protected:
      
        Generator freqGen_;
        Generator pwmGen_;
        
        TonicFrames freqFrames_;
        TonicFrames pwmFrames_;
        
        double phaseAccum_;
      
        void computeSynthesisBlock( const SynthesisContext_ &context );
      
      public:
      
        RectWave_();
        
        void setFrequencyGenerator( Generator gen ){
          freqGen_ = gen;
        }
        
        void setPwmGenerator( Generator gen ){
          pwmGen_ = gen;
        }
        
        
    };
      
    inline void RectWave_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      // tick freq and pwm
      freqGen_.tick(freqFrames_, context);
      pwmGen_.tick(pwmFrames_, context);
      
      const TonicFloat rateConstant =  TONIC_RECT_RES / Tonic::sampleRate();

      TonicFloat *outptr = &outputFrames_[0];
      TonicFloat *freqptr = &freqFrames_[0];
      TonicFloat *pwmptr = &pwmFrames_[0];
      
      FastPhasor sd;
      
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
      TonicInt32 offs, msbi = sd.i[1];
      double ps = phaseAccum_ + BIT32DECPT;
      for ( unsigned int i=0; i<outputFrames_.frames(); i++ ) {
        
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
    
    
    // -------------------------------------
    
    
    //! Bandlimited Rect Wave
    class RectWaveBL_ : public BLEPOscillator_
    {
      
    protected:
      
      // Input Generators
      Generator   pwmGen_;
      TonicFrames pwmFrames_;
          
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    public:
      
      RectWaveBL_();
      void setPWMGen(Generator gen) { pwmGen_ = gen; };

    };
    
    inline void RectWaveBL_::computeSynthesisBlock(const Tonic_::SynthesisContext_ &context)
    {
      
      static const TonicFloat rateConstant =  1.0f / Tonic::sampleRate();
      
      // tick freq and pwm
      freqGen_.tick(freqFrames_, context);
      pwmGen_.tick(pwmFrames_, context);
      
      TonicFloat *outptr = &outputFrames_[0];
      TonicFloat *freqptr = &freqFrames_[0];
      TonicFloat *pwmptr = &pwmFrames_[0];
            
      // pre-multiply rate constant for speed
#ifdef USE_APPLE_ACCELERATE
      vDSP_vsmul(freqptr, 1, &rateConstant, freqptr, 1, kSynthesisBlockSize);
#else
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
        *freqptr++ *= rateConstant;
      }
      freqptr = &freqFrames_[0];
#endif
            
      // TODO: Maybe do this using a fast phasor for wraparound speed
      for (unsigned int i=0; i<kSynthesisBlockSize; i++, pwmptr++, freqptr++, outptr++){
        
        phase_ += *freqptr;
        
        // add BLEP at end
        if (phase_ >= 1.0)
        {
          phase_ -= 1.0;
          accum_ = 0.0f;
          addBLEP(phase_/(*freqptr), 1.0f);
        }
        
        // add BLEP when we exceed pwm
        if (accum_ == 0.f && phase_ > *pwmptr)
        {
          accum_ = 1.0f;
          addBLEP((phase_ - (*pwmptr))/(*freqptr),-1.0f);
        }
        
        *outptr = accum_;
        
        // add BLEP buffer contents
        if (nInit_ > 0)
        {
          *outptr += ringBuf_[iBuffer_];
          nInit_--;
          if (++iBuffer_ >= lBuffer_) iBuffer_ = 0;
        }
        
        // remove DC offset
        *outptr = (*outptr * 2.f) - 1.f;
        
      }

    }
    
  }
  
  //! Quick-and-dirty rectangular wave
  /*!
      WARNING: Is NOT anti-aliased!! Best for use as LFO, or if you just don't care :)
  */
  class RectWave : public TemplatedGenerator<Tonic_::RectWave_>{
    
  public:
    
    //! Set the frequency of the waveform
    TONIC_MAKE_GEN_SETTERS(RectWave, freq, setFrequencyGenerator);
    
    //! Set the pulse width of the rectangle. Input should be clipped between 0-1
    TONIC_MAKE_GEN_SETTERS(RectWave, pwm, setPwmGenerator);

  };
  
  //! Bandlimited rectangular wave
  class RectWaveBL : public TemplatedGenerator<Tonic_::RectWaveBL_>{
    
  public:
    
    //! Set the frequency of the waveform
    TONIC_MAKE_GEN_SETTERS(RectWaveBL, freq, setFreqGen);
    
    //! Set the pulse width of the rectangle. Input should be clipped between 0-1
    TONIC_MAKE_GEN_SETTERS(RectWaveBL, pwm, setPWMGen);
  };
  
}

#endif


