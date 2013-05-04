//
//  FlexToothWave.h
//  Tonic 
//
//  Created by Nick Donaldson on 3/2/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__FlexToothWave__
#define __Tonic__FlexToothWave__

#include "Generator.h"

#define TONIC_FLEXTOOTH_RES 4096

namespace Tonic {
  
  namespace Tonic_ {

    class FlexToothWave_ : public Generator_{
      
    protected:
      
      Generator freqGen_;
      Generator slopeGen_;
      
      TonicFrames freqFrames_;
      TonicFrames slopeFrames_;
      
      double phaseAccum_;
      
    public:
      FlexToothWave_();
      ~FlexToothWave_();
      
      void setFrequencyGenerator( Generator gen ){
        freqGen_ = gen;
      }
      
      void setSlopeGenerator( Generator gen ){
        slopeGen_ = gen;
      }
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    };
    
    inline void FlexToothWave_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      // tick freq and slope inputs
      freqGen_.tick(freqFrames_, context);
      slopeGen_.tick(slopeFrames_, context);
      
      // calculate the output wave
      TonicFloat const rateConstant = TONIC_FLEXTOOTH_RES/Tonic::sampleRate();
      
      TonicFloat slope, frac, phase;
      TonicFloat *outptr = &synthesisBlock_[0];
      TonicFloat *freqptr = &freqFrames_[0];
      TonicFloat *slopeptr = &slopeFrames_[0];
      
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
      TonicInt32 offs, msbi = sd.i[1];
      double ps = phaseAccum_ + BIT32DECPT;
      for ( unsigned int i=0; i<synthesisBlock_.frames(); i++ ) {
        
        // update the slope
        slope = clamp(*slopeptr++, 0.0f, 1.0f) * TONIC_FLEXTOOTH_RES;
        
        sd.d = ps;
        ps += *freqptr++;
        offs = sd.i[1] & (TONIC_FLEXTOOTH_RES-1);
        sd.i[1] = msbi;
        frac = sd.d - BIT32DECPT;
        
        phase = offs + frac;
        
        // TODO: this is still a bit slow. Maybe find a way to do it with a real table lookup
        // on the rising edge
        if (phase < slope){
          *outptr++ = slope == 0 ? 0 : (phase/slope)*2.0f - 1.0f;
        }
        // on the falling edge
        else{
          *outptr++ = slope == 1 ? 0 : (1.0f - ((phase - slope)/(TONIC_FLEXTOOTH_RES - slope))) * 2.0f - 1.0f;
        }
                
      }
      
      sd.d = BIT32DECPT * TONIC_FLEXTOOTH_RES;
      msbi = sd.i[1];
      sd.d = ps + (BIT32DECPT * TONIC_FLEXTOOTH_RES - BIT32DECPT);
      sd.i[1] = msbi;
      phaseAccum_ = sd.d - BIT32DECPT * TONIC_FLEXTOOTH_RES;
      
    }
    
  }
  
  //! Quick-and-dirty variable sawtooth-triangle
  /*!
      Depending on slope input, this generator can either be a rising sawtooth, a falling sawtooth,
      or anything in between (including a triangle wave).
   
      Slopes:
        0   = falling sawtooth
        0.5 = triangle
        1   = rising sawtooth
      
      Not anti-aliased, so really best as an LFO. Can be used as an audio sawtooth oscillator in a pinch
      or if you don't mind some aliasing distortion.
  */
  class FlexToothWave : public TemplatedGenerator<Tonic_::FlexToothWave_>{
    
  public:
    
    createGeneratorSetters(FlexToothWave, freq, setFrequencyGenerator);
    createGeneratorSetters(FlexToothWave, slope, setSlopeGenerator);

  };
}

#endif /* defined(__Tonic__SawWave__) */


