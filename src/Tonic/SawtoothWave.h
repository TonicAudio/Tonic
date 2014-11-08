//
//  SawtoothWave.h
//  Tonic 
//
//  Created by Nick Donaldson on 3/2/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_ANGULARWAVE_H
#define TONIC_ANGULARWAVE_H

#include "Generator.h"
#include "BLEPOscillator.h"

#define TONIC_SAW_RES 4096

namespace Tonic {
  
  namespace Tonic_ {

    // Generator_ class to generate hard-edge rising saw, falling saw, triangle, or any angle in-between.
    
    class AngularWave_ : public Generator_{
      
    protected:
      
      Generator freqGen_;
      Generator slopeGen_;
      
      TonicFrames freqFrames_;
      TonicFrames slopeFrames_;
      
      double phaseAccum_;
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    public:
      AngularWave_();
      
      void setFrequencyGenerator( Generator gen ){
        freqGen_ = gen;
      }
      
      void setSlopeGenerator( Generator gen ){
        slopeGen_ = gen;
      }
      
      
    };
    
    inline void AngularWave_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      // tick freq and slope inputs
      freqGen_.tick(freqFrames_, context);
      slopeGen_.tick(slopeFrames_, context);
      
      // calculate the output wave
      TonicFloat const rateConstant = TONIC_SAW_RES/Tonic::sampleRate();
      
      TonicFloat slope, frac, phase;
      TonicFloat *outptr = &outputFrames_[0];
      TonicFloat *freqptr = &freqFrames_[0];
      TonicFloat *slopeptr = &slopeFrames_[0];
      
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
        
        // update the slope
        slope = clamp(*slopeptr++, 0.0f, 1.0f) * TONIC_SAW_RES;
        
        sd.d = ps;
        ps += *freqptr++;
        offs = sd.i[1] & (TONIC_SAW_RES-1);
        sd.i[1] = msbi;
        frac = sd.d - BIT32DECPT;
        
        phase = offs + frac;
        
        // TODO: this is still a bit slow. Maybe find a way to do it with a real table lookup
        
        // on the rising edge
        if (phase < slope){
          *outptr++ = (phase/slope)*2.0f - 1.0f;
        }
        // on the falling edge
        else{
          *outptr++ = (1.0f - ((phase - slope)/(TONIC_SAW_RES - slope))) * 2.0f - 1.0f;
        }
                
      }
      
      sd.d = BIT32DECPT * TONIC_SAW_RES;
      msbi = sd.i[1];
      sd.d = ps + (BIT32DECPT * TONIC_SAW_RES - BIT32DECPT);
      sd.i[1] = msbi;
      phaseAccum_ = sd.d - BIT32DECPT * TONIC_SAW_RES;
      
    }
    
    // -----------------------------------
    
    //! Anti-aliased BLEP sawtooth
    class SawtoothWaveBL_ : public BLEPOscillator_
    {
      
    protected:
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    };
    
    inline void SawtoothWaveBL_::computeSynthesisBlock(const Tonic_::SynthesisContext_ &context)
    {
      
      static const TonicFloat rateConstant =  1.0f / Tonic::sampleRate();
      
      // tick freq and pwm
      freqGen_.tick(freqFrames_, context);
      
      TonicFloat *outptr = &outputFrames_[0];
      TonicFloat *freqptr = &freqFrames_[0];
      
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
      for (unsigned int i=0; i<kSynthesisBlockSize; i++, freqptr++, outptr++){
        
        phase_ += *freqptr;
        
        // add BLEP at end
        if (phase_ >= 1.0)
        {
          phase_ -= 1.0;
          accum_ = 0.0f;
          addBLEP(phase_/(*freqptr), 1.0f);
        }
        
        *outptr = (TonicFloat)phase_;
        
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
  
  //! Quick-and-dirty sawtooth oscillator
  /*!
      Not anti-aliased, so really best as an LFO. Can be used as an audio sawtooth oscillator in a pinch
      or if you don't mind some aliasing distortion.
  */
  class SawtoothWave : public TemplatedGenerator<Tonic_::AngularWave_>
  {
    
    public:
      
      TONIC_MAKE_GEN_SETTERS(SawtoothWave, freq, setFrequencyGenerator);
      
      //! set whether it's a descending sawtooth (default) or ascending
      SawtoothWave & isAscending(bool ascending){
        gen()->setSlopeGenerator(FixedValue(ascending ? 1.f : 0.f));
        return *this;
      }
    
  };
  
  //! Bandlimited sawtooth generator
  class SawtoothWaveBL : public TemplatedGenerator<Tonic_::SawtoothWaveBL_>
  {
    
    public:
      TONIC_MAKE_GEN_SETTERS(SawtoothWaveBL, freq, setFreqGen);

  };
  
  
}

#endif


