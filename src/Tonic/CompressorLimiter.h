//
//  CompressorLimiter.h
//  Tonic 
//
//  Created by Nick Donaldson on 3/24/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_COMPRESSOR_H
#define TONIC_COMPRESSOR_H

#include "Effect.h"
#include "DelayUtils.h"
#include "FilterUtils.h"

namespace Tonic {
  
  namespace Tonic_ {

    class Compressor_ : public Effect_{
      
    protected:
      
      // Can be overridden for sidechaining
      Generator amplitudeInput_;
      
      ControlGenerator makeupGainGen_;
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
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    public:
      
      Compressor_();

      // Base class methods overridden here for specialized input behavior
      void setInput( Generator input );
      void tick(TonicFrames &frames, const SynthesisContext_ & context );
      void tickThrough(TonicFrames & inFrames, TonicFrames & outFrames, const SynthesisContext_ & context);
      
      // setters
      void setAudioInput( Generator gen );
      void setAmplitudeInput( Generator gen );
      
      void setMakeupGain( ControlGenerator gen ) { makeupGainGen_ = gen; };
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
      
      if (context.forceNewOutput || lastFrameIndex_ != context.elapsedFrames){
        amplitudeInput_.tick(ampInputFrames_, context); // get amp input frames
      }
      Effect_::tick(frames, context);
      
    }
    
    inline void Compressor_::tickThrough(TonicFrames & inFrames, TonicFrames & outFrames, const SynthesisContext_ & context){
      ampInputFrames_.copy(inFrames);
      Effect_::tickThrough(inFrames, outFrames, context);
    }
    
    inline void Compressor_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      // Tick all scalar parameters
      float attackCoef = t60ToOnePoleCoef(max(0,attackGen_.tick(context).value));
      float releaseCoef = t60ToOnePoleCoef(max(0, releaseGen_.tick(context).value));
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
      unsigned int nChannels = outputFrames_.channels();
      TonicFloat ampInputValue, gainValue, gainTarget;
      TonicFloat * outptr = &outputFrames_[0];
      TonicFloat * dryptr = &dryFrames_[0];
      ampData = &ampInputFrames_[0];
      
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
        
        // Tick input into lookahead delay and get amplitude input value - max of left/right
        ampInputValue = 0;
        for (unsigned int i=0; i<nChannels; i++){
          lookaheadDelayLine_.tickIn(*dryptr++, i);
          ampInputValue = max(ampInputValue,*ampData++);
        }
        
        // Smooth amplitude input
        if (ampInputValue >= ampEnvValue_){
          onePoleLPFTick(ampInputValue, ampEnvValue_, attackCoef);
        }
        else {
          onePoleLPFTick(ampInputValue, ampEnvValue_, releaseCoef);
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
          onePoleLPFTick(gainValue, gainEnvValue_, attackCoef);
        }
        else {
          onePoleLPFTick(gainValue, gainEnvValue_, releaseCoef);
        }
        
        // apply gain
        for (unsigned int i=0; i<nChannels; i++){
          *outptr++ = lookaheadDelayLine_.tickOut(lookaheadTime,i) * gainEnvValue_;
        }
        
        lookaheadDelayLine_.advance();
      }
      
      TonicFloat makeupGain = max(0.f, makeupGainGen_.tick(context).value);
      outptr = &outputFrames_[0];
      
      #ifdef USE_APPLE_ACCELERATE
      vDSP_vsmul(outptr, 1, &makeupGain, outptr, 1, outputFrames_.size());
      #else
      for (unsigned int i=0; i<outputFrames_.size(); i++){
        *outptr++ *= makeupGain;
      }
      #endif
      
      if (isLimiter_){
        
        // clip to threshold in worst case (minor distortion introduced but much preferable to wrapping distortion)
        #ifdef USE_APPLE_ACCELERATE
        float negThresh = -threshold;
        vDSP_vclip(&outputFrames_[0], 1, &negThresh, &threshold, &outputFrames_[0], 1, outputFrames_.size());
        #else
        outptr = &outputFrames_[0];
        for (unsigned int i=0; i<outputFrames_.size(); i++, outptr++){
          *outptr = clamp(*outptr, -threshold, threshold);
        }
        #endif
        
      }
      
    }
    
  }
  
  class Compressor : public TemplatedEffect<Compressor, Tonic_::Compressor_>{
    
  public:
    
    Compressor(float threshold = 0.5, float ratio = 2, float attack = 0.001, float release = 0.05, float lookahead = 0.001);

    //! Input for audio to be compressed
    Compressor & audioInput( Generator input ){
      this->gen()->setInput( input );
      return *this;
    }
    
    //! Input for audio for compression amplitude envelope
    Compressor & sidechainInput( Generator input ){
      this->gen()->setAmplitudeInput(input);
      return *this;
    }
    
    void setIsStereo( bool isStereo ){
      this->gen()->setIsStereo(isStereo);
    }
    
    TONIC_MAKE_CTRL_GEN_SETTERS(Compressor, attack, setAttack);
    TONIC_MAKE_CTRL_GEN_SETTERS(Compressor, release, setRelease);
    TONIC_MAKE_CTRL_GEN_SETTERS(Compressor, threshold, setThreshold); // LINEAR - use dBToLin to convert from dB
    TONIC_MAKE_CTRL_GEN_SETTERS(Compressor, ratio, setRatio);
    TONIC_MAKE_CTRL_GEN_SETTERS(Compressor, lookahead, setLookahead);
    TONIC_MAKE_CTRL_GEN_SETTERS(Compressor, makeupGain, setMakeupGain);


    // TODO: option for RMS
    
  };
  
  //! Special case of compressor where ratio is fixed to infinite and attack time is fixed to 0.1 ms
  //  Used for peak limiting
  class Limiter : public TemplatedEffect<Limiter, Tonic_::Compressor_>{
    
  public:
    
    Limiter();
    
    void setIsStereo( bool isStereo ){
      this->gen()->setIsStereo(isStereo);
    }
    
    TONIC_MAKE_CTRL_GEN_SETTERS(Limiter, release, setRelease);
    TONIC_MAKE_CTRL_GEN_SETTERS(Limiter, threshold, setThreshold);
    TONIC_MAKE_CTRL_GEN_SETTERS(Limiter, lookahead, setLookahead);
    TONIC_MAKE_CTRL_GEN_SETTERS(Limiter, makeupGain, setMakeupGain);
    
  };

}

#endif


