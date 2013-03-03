//
//  FlexToothWave.h
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

#ifndef __Tonic__FlexToothWave__
#define __Tonic__FlexToothWave__

#include "Generator.h"
#include "FixedValue.h"

namespace Tonic {
  
  namespace Tonic_ {

    class FlexToothWave_ : public Generator_{
      
    protected:
      
      Generator freqGen_;
      Generator slopeGen_;
      
      TonicFrames freqFrames_;
      TonicFrames slopeFrames_;
      
      float phaseAccum_;
      
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
      lockMutex();
      freqGen_.tick(freqFrames_, context);
      slopeGen_.tick(slopeFrames_, context);
      unlockMutex();
      
      // calculate the output wave
      TonicFloat const T = 1.0f/Tonic::sampleRate();
      
      TonicFloat period, normPhase, slope;
      TonicFloat *outptr = &synthesisBlock_[0];
      TonicFloat *freqptr = &freqFrames_[0];
      TonicFloat *slopeptr = &slopeFrames_[0];
      
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
        
        // update the slope
        slope = clamp(*slopeptr++, 0.0f, 1.0f);
        
        // update the current period in seconds
        period = 1.0f/(max(std::numeric_limits<float>::min(),*freqptr++));
        
        // update the current phase in seconds
        phaseAccum_ += T;
        
        // wrap the phase
        phaseAccum_ = fmodf(phaseAccum_, period);
        normPhase = phaseAccum_/period;
        
        // compute output sample
        
        // on the rising edge
        if (normPhase < slope){
          *outptr++ = slope == 0 ? 0 : (normPhase/slope)*2.0f - 1.0f;
        }
        // on the falling edge
        else{
          *outptr++ = slope == 1 ? 0 : (1.0f - ((normPhase - slope)/(1.0f - slope))) * 2.0f - 1.0f;
        }
        
      }
      
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


