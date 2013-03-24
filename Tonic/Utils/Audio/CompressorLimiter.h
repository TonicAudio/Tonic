//
//  CompressorLimiter.h
//  Tonic 
//
//  Created by Nick Donaldson on 3/24/13.
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

#ifndef __Tonic__Compressor__
#define __Tonic__Compressor__

#include "Effect.h"
#include "DelayUtils.h"
#include "FilterUtils.h"

namespace Tonic {
  
  namespace Tonic_ {

    class Compressor_ : public Effect_{
      
    protected:
      
      // Can be overridden for sidechaining
      Generator amplitudeInput_;
      
      ControlGenerator attackGen_;
      ControlGenerator releaseGen_;
      ControlGenerator threshGen_;
      ControlGenerator ratioGen_;
      ControlGenerator lookaheadGen_;
          
      DelayLine lookaheadDelayLine_;
      
      TonicFrames ampInputFrames_;

      TonicFloat ampEnvValue_;
      TonicFloat gainEnvValue_;
      
    public:
      
      Compressor_();
      ~Compressor_();
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
      // setters
      void setAmplitudeInput( Generator gen ) { amplitudeInput_ = gen; };
      void setAttack( ControlGenerator gen ) { attackGen_ = gen; };
      void setRelease( ControlGenerator gen ) { releaseGen_ = gen; };
      void setThreshold( ControlGenerator gen ) { threshGen_ = gen; };
      void setRatio( ControlGenerator gen ) { ratioGen_ = gen; };
      void setLookahead( ControlGenerator gen ) { lookaheadGen_ = gen; };
      
    };
    
    inline void Compressor_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      // Tick all scalar parameters
      float attackCoef = msToTc(min(0,attackGen_.tick(context).value));
      float releaseCoef = msToTc(max(0, releaseGen_.tick(context).value));
      float threshold = threshGen_.tick(context).value;
      float ratio = ratioGen_.tick(context).value;
      float lookaheadTime = lookaheadGen_.tick(context).value;
      
      // Tick in amplitude frames
      amplitudeInput_.tick(ampInputFrames_, context);
      
      // Absolute value of amplitude frames in prep for amp envelope
      TonicFloat * ampData = &ampInputFrames_[0];
      
      #ifdef USE_APPLE_ACCELERATE
      vDSP_vabs(ampData, 1, ampData, 1, kSynthesisBlockSize*2);
      #else
      for (unsigned int i=0; i<kSynthesisBlockSize*2; i++, ampData++){
        *ampData = fabsf(*ampData);
      }
      #endif
      
      // Tick in input frames
      input_.tick(synthesisBlock_, context);
      
      // Iterate through samples
      TonicFloat ampInputValue, gainValue, gainTarget;
      TonicFloat * data = &synthesisBlock_[0];
      ampData = &ampInputFrames_[0];
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
        
        // Tick input into lookahead delay
        lookaheadDelayLine_.tickIn(*data, 0);
        lookaheadDelayLine_.tickIn(*(data+1), 1);
        
        // Take average of L+R amplitude input
        ampInputValue = *ampData++;
        ampInputValue += *ampData++;
        ampInputValue /= 2.0f;
        
        // Smooth amplitude input
        if (ampInputValue >= ampEnvValue_){
          onePoleTick(ampInputValue, ampEnvValue_, attackCoef);
        }
        else {
          onePoleTick(ampInputValue, ampEnvValue_, releaseCoef);
        }
        
        // Calculate gain value
        
        if (ampEnvValue_ < threshold){
          gainValue = 1.0f;
        }
        else{
          // compensate for ratio
          gainTarget = threshold + ((ampEnvValue_ - threshold)/ratio);
          gainValue = gainTarget/ampEnvValue_;
        }
        
        // Smooth gain value
        if (gainValue >= gainEnvValue_){
          onePoleTick(gainValue, gainEnvValue_, attackCoef);
        }
        else {
          onePoleTick(gainValue, gainEnvValue_, releaseCoef);
        }
        
        // apply gain
        *data++ = lookaheadDelayLine_.tickOut(0) * gainEnvValue_;
        *data++ = lookaheadDelayLine_.tickOut(1) * gainEnvValue_;
        
        lookaheadDelayLine_.advance(lookaheadTime);
      }
      
    }
    
  }
  
  class Compressor : public TemplatedEffect<Compressor, Tonic_::Compressor_>{
    
  public:
    
    //! default input method sets both audio signal and amplitude signal as input
    //! so incoming signal is compressed based on its own amplitude
    inline Compressor & input( Generator input ){
      this->gen()->setInput( input );
      this->gen()->setAmplitudeInput( input );
      return *this;
    }
    
    //! Input for audio to be compressed
    createGeneratorSetters(Compressor, audioInput, setInput);
    
    //! Input for amplitude to drive compression
    createGeneratorSetters(Compressor, sidechainInput, setAmplitudeInput);
    
    createControlGeneratorSetters(Compressor, attack, setAttack);
    createControlGeneratorSetters(Compressor, release, setRelease);
    createControlGeneratorSetters(Compressor, threshold, setThreshold); // LINEAR - use dBToLin to convert from dB
    createControlGeneratorSetters(Compressor, ratio, setRatio);
    createControlGeneratorSetters(Compressor, lookahead, setLookahead);

    // TODO: RMS

    
  };
  
  //! Special case of compressor where ratio is fixed to infinite and attack time is fixed to 0.1 ms
  //  Used for peak limiting
  class Limiter : public TemplatedEffect<Limiter, Tonic_::Compressor_>{
    
  public:
    
    Limiter() {
      gen()->setAttack(ControlValue(0.0001));
      gen()->setLookahead(ControlValue(0.001));
      gen()->setRelease(ControlValue(0.030));
      gen()->setThreshold(ControlValue(dBToLin(-1.0)));
      gen()->setRatio(ControlValue(std::numeric_limits<float>::max()));
    }
    
    //! default input method sets both audio signal and amplitude signal as input
    //! so incoming signal is compressed based on its own amplitude
    inline Limiter & input( Generator input ){
      this->gen()->setInput( input );
      this->gen()->setAmplitudeInput( input );
      return *this;
    }
    
    createControlGeneratorSetters(Limiter, release, setRelease);
    createControlGeneratorSetters(Limiter, threshold, setThreshold);
    createControlGeneratorSetters(Limiter, lookahead, setLookahead);
    
  };

}

#endif /* defined(__Tonic__Compressor__) */


