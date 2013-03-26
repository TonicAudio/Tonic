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
      
      bool isLimiter_;
      
    public:
      
      Compressor_();
      ~Compressor_();

      // Overridden here for specialized input behavior
      void setInput( Generator input );
      void tick(TonicFrames &frames, const SynthesisContext_ &context );
      void tickThrough(TonicFrames &frames);      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
      // setters
      void setAudioInput( Generator gen );
      void setAmplitudeInput( Generator gen );
      
      void setAttack( ControlGenerator gen ) { attackGen_ = gen; };
      void setRelease( ControlGenerator gen ) { releaseGen_ = gen; };
      void setThreshold( ControlGenerator gen ) { threshGen_ = gen; };
      void setRatio( ControlGenerator gen ) { ratioGen_ = gen; };
      void setLookahead( ControlGenerator gen ) { lookaheadGen_ = gen; };
      
      //! Set whether is a limiter - limiters will hard clip to threshold in worst case
      void setIsLimiter( bool isLimiter ) { isLimiter_ = isLimiter; };
      
      //! Externally set whether operates on one or two channels
      void setIsStereo( bool isStereo );
      
    };
    
    inline void Compressor_::tick(TonicFrames &frames, const SynthesisContext_ &context ){
      
      // check context to see if we need new frames
      if (context.elapsedFrames == 0 || lastFrameIndex_ != context.elapsedFrames){
        lockMutex();
        input_.tick(dryFrames_, context); // get input frames
        amplitudeInput_.tick(ampInputFrames_, context); // get amp input frames
        computeSynthesisBlock(context);
        unlockMutex();
        lastFrameIndex_ = context.elapsedFrames;
      }
      
      // copy synthesis block to frames passed in
      frames.copy(synthesisBlock_);
      
#ifdef TONIC_DEBUG
      if(frames(0,0) != frames(0,0)){
        Tonic::error("Effect_::tick NaN detected.");
      }
#endif
      
    }
    
    inline void Compressor_::tickThrough(TonicFrames &frames){
      dryFrames_.copy(frames);
      ampInputFrames_.copy(frames);
      lockMutex();
      computeSynthesisBlock(SynthesisContext_());
      unlockMutex();
      frames.copy(synthesisBlock_);
    }
    
    inline void Compressor_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      // Tick all scalar parameters
      float attackCoef = t60ToTau(max(0,attackGen_.tick(context).value));
      float releaseCoef = t60ToTau(max(0, releaseGen_.tick(context).value));
      float threshold = max(0,threshGen_.tick(context).value);
      float ratio = max(0,ratioGen_.tick(context).value);
      float lookaheadTime = max(0,lookaheadGen_.tick(context).value);
      
      // Absolute value of amplitude frames in prep for amp envelope
      TonicFloat * ampData = &ampInputFrames_[0];
      
      #ifdef USE_APPLE_ACCELERATE
      vDSP_vabs(ampData, 1, ampData, 1, ampInputFrames_.size());
      #else
      for (unsigned int i=0; i<ampInputFrames_.size(); i++, ampData++){
        *ampData = fabsf(*ampData);
      }
      #endif
      
      // Iterate through samples
      unsigned int nChannels = synthesisBlock_.channels();
      TonicFloat ampInputValue, gainValue, gainTarget;
      TonicFloat * outptr = &synthesisBlock_[0];
      TonicFloat * dryptr = &dryFrames_[0];
      ampData = &ampInputFrames_[0];
      
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
        
        // Tick input into lookahead delay
        for (unsigned int i=0; i<nChannels; i++){
          lookaheadDelayLine_.tickIn(*dryptr++, i);
        }
        
        // Get amplitude input value - max of left/right
        ampInputValue = 0;
        for (unsigned int i=0; i<nChannels; i++){
          ampInputValue = max(ampInputValue,*ampData++);
        }
        
        // Smooth amplitude input
        if (ampInputValue >= ampEnvValue_){
          onePoleTick(ampInputValue, ampEnvValue_, attackCoef);
        }
        else {
          onePoleTick(ampInputValue, ampEnvValue_, releaseCoef);
        }
        
        // Calculate gain value
        
        if (ampEnvValue_ <= threshold){
          gainValue = 1.0f;
        }
        else{
          // compensate for ratio
          gainTarget = threshold + ((ampEnvValue_ - threshold)/ratio);
          gainValue = gainTarget/ampEnvValue_;
        }
        
        // Smooth gain value
        if (gainValue <= gainEnvValue_){
          onePoleTick(gainValue, gainEnvValue_, attackCoef);
        }
        else {
          onePoleTick(gainValue, gainEnvValue_, releaseCoef);
        }
        
        // apply gain
        for (unsigned int i=0; i<nChannels; i++){
          *outptr++ = lookaheadDelayLine_.tickOut(i) * gainEnvValue_;
        }
        
        lookaheadDelayLine_.advance(lookaheadTime);
      }
      
      if (isLimiter_){
        
        // clip to threshold in worst case (minor distortion introduced but much preferable to wrapping distortion)
        #ifdef USE_APPLE_ACCELERATE
        float negThresh = -threshold;
        vDSP_vclip(&synthesisBlock_[0], 1, &negThresh, &threshold, &synthesisBlock_[0], 1, synthesisBlock_.size());
        #else
        outptr = &synthesisBlock_[0];
        for (unsigned int i=0; i<synthesisBlock_.size(); i++, outptr++){
          *outptr = clamp(*outptr, -threshold, threshold);
        }
        #endif
        
      }
      
    }
    
  }
  
  class Compressor : public TemplatedEffect<Compressor, Tonic_::Compressor_>{
    
  public:

    //! Input for audio to be compressed
    Compressor & audioInput( Generator input ){
      this->gen()->lockMutex();
      this->gen()->setInput( input );
      this->gen()->unlockMutex();
      return *this;
    }
    
    //! Input for audio for compression amplitude envelope
    Compressor & sidechainInput( Generator input ){
      this->gen()->lockMutex();
      this->gen()->setAmplitudeInput(input);
      this->gen()->unlockMutex();
      return *this;
    }

    createControlGeneratorSetters(Compressor, attack, setAttack);
    createControlGeneratorSetters(Compressor, release, setRelease);
    createControlGeneratorSetters(Compressor, threshold, setThreshold); // LINEAR - use dBToLin to convert from dB
    createControlGeneratorSetters(Compressor, ratio, setRatio);
    createControlGeneratorSetters(Compressor, lookahead, setLookahead);

    // TODO: option for RMS
    
  };
  
  //! Special case of compressor where ratio is fixed to infinite and attack time is fixed to 0.1 ms
  //  Used for peak limiting
  class Limiter : public TemplatedEffect<Limiter, Tonic_::Compressor_>{
    
  public:
    
    Limiter();
    
    void setIsStereo( bool isStereo ){
      this->gen()->lockMutex();
      this->gen()->setIsStereo(isStereo);
      this->gen()->unlockMutex();
    }
    
    //! default input method sets both audio signal and amplitude signal as input
    //! so incoming signal is compressed based on its own amplitude
    Limiter & input( Generator input ){
      this->gen()->lockMutex();
      this->gen()->setInput( input );
      this->gen()->setAmplitudeInput( input );
      this->gen()->setIsStereoOutput( input.isStereoOutput() );
      this->gen()->setIsStereoInput( input.isStereoOutput() );
      this->gen()->unlockMutex();
      return *this;
    }
    
    createControlGeneratorSetters(Limiter, release, setRelease);
    createControlGeneratorSetters(Limiter, threshold, setThreshold);
    createControlGeneratorSetters(Limiter, lookahead, setLookahead);
    
  };

}

#endif /* defined(__Tonic__Compressor__) */


