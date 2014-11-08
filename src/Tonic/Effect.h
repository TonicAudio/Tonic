//
//  Effect.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/20/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_EFFECT_H
#define TONIC_EFFECT_H

#include "Generator.h"

namespace Tonic {
  
  namespace Tonic_ {
    
    class Effect_ : public Generator_{

      protected:
        
        Generator input_;
        TonicFrames dryFrames_;
        
        ControlGenerator bypassGen_;
        bool isStereoInput_;
        
      public:
        
        Effect_();
              
        void setBypassCtrlGen( ControlGenerator gen ){ bypassGen_ = gen; };

        virtual void setInput( Generator input ) { input_ = input; };
        
        //! set stereo/mono - changes number of channels in dryFrames_
        /*!
            subclasses should call in constructor to determine input channel layout
        */
        virtual void setIsStereoInput( bool stereo );
        
        bool isStereoInput() { return isStereoInput_; };

        // --- Tick methods ---
        
        virtual void tick(TonicFrames &frames, const SynthesisContext_ &context );
        
        //! Apply effect directly to passed in frames (output in-place)
        /*!
            DO NOT mix calls to tick() with calls to tickThrough().
        */
        virtual void tickThrough( TonicFrames & inFrames, TonicFrames & outFrames, const SynthesisContext_ & context );

    };
    
    
    inline void Effect_::setIsStereoInput(bool stereo)
    {
      if (stereo != isStereoInput_){
        dryFrames_.resize(kSynthesisBlockSize, stereo ? 2 : 1, 0);
      }
      isStereoInput_ = stereo;
    }
    
    // Overridden tick - pre-ticks input to fill dryFrames_.
    // subclasses don't need to tick input - dryFrames_ contains "dry" input by the time
    // computeSynthesisBlock() is called
    inline void Effect_::tick(TonicFrames &frames, const SynthesisContext_ &context ){
      
      // check context to see if we need new frames
      if (context.elapsedFrames == 0 || lastFrameIndex_ != context.elapsedFrames){
        
        // get dry input frames
        input_.tick(dryFrames_, context);
        
        computeSynthesisBlock(context);

        // bypass processing - still need to compute block so all generators stay in sync
        bool bypass = bypassGen_.tick(context).value != 0.f;
        if (bypass){
          outputFrames_.copy(dryFrames_);
        }
        
        lastFrameIndex_ = context.elapsedFrames;
      }
      
      // copy synthesis block to frames passed in
      frames.copy(outputFrames_);
      
#ifdef TONIC_DEBUG
      if(!isfinite(frames(0,0))){
        Tonic::error("Effect_::tick NaN or inf detected.");
      }
#endif
      
    }
    
    inline void Effect_::tickThrough(TonicFrames & inFrames, TonicFrames & outFrames, const SynthesisContext_ & context){

        // Do not check context here, assume each call should produce new output.
        
        dryFrames_.copy(inFrames);
        computeSynthesisBlock(context);
        
        // bypass processing - still need to compute block so all generators stay in sync
        bool bypass = bypassGen_.tick(context).value != 0.f;
        if (bypass){
          outFrames.copy(dryFrames_);
        }
        else{
          outFrames.copy(outputFrames_);
        }
      
    }
  }
  
  template<class EffectType, class EffectType_>
  class TemplatedEffect : public TemplatedGenerator<EffectType_>
  {
    
    public:
          
      // This cast is not safe - up to implementation to ensure that templated EffectType_ is actually an Effect_ subclass
      virtual EffectType & input( Generator input ){
        this->gen()->setInput( input );
        return static_cast<EffectType&>(*this);
      }
      
      void tickThrough( TonicFrames & inFrames, const Tonic_::SynthesisContext_ & context){ // ticks in-place
        this->gen()->tickThrough(inFrames, inFrames, context);
      }
      
      void tickThrough(TonicFrames & inFrames, TonicFrames & outFrames, const Tonic_::SynthesisContext_ & context){
        this->gen()->tickThrough(inFrames, outFrames, context);
      }
      
      void setIsStereoInput( bool isStereoInput ){
        this->gen()->setIsStereoInput(isStereoInput);
      }
      
      TONIC_MAKE_CTRL_GEN_SETTERS(EffectType, bypass, setBypassCtrlGen);

  };
  
  // signal flow operator - sets lhs as input to rhs
  template<class EffectType, class EffectType_>
  static EffectType operator>>(Generator lhs, TemplatedEffect<EffectType, EffectType_> rhs){
    return rhs.input( lhs );
  }

  
  // ------------------------------------
  //        Wet/Dry mix effects
  // ------------------------------------
  
  namespace Tonic_
  {
    
    // WetDryEffect_ extends the basic functionality of Effect_ but also adds a wet/dry mix to the basic tick() cycle
    class WetDryEffect_ : public Effect_
    {
      protected:
      
        Generator  dryLevelGen_;
        Generator  wetLevelGen_;
        TonicFrames mixWorkspace_;
      
      public:
      
        WetDryEffect_();
      
        void setDryLevelGen( Generator gen ){ dryLevelGen_ = gen; };
        void setWetLevelGen( Generator gen ){ wetLevelGen_ = gen; };
      
      // --- Tick methods ---
      
      virtual void tick(TonicFrames &frames, const SynthesisContext_ &context );
      
      //! Apply effect directly to passed in frames (output in-place)
      /*!
          DO NOT mix calls to tick() with calls to tickThrough().
       */
      virtual void tickThrough( TonicFrames & inFrames, TonicFrames & outFrames, const SynthesisContext_ & context );

    };
    
    // Overridden tick - pre-ticks input to fill dryFrames_.
    // subclasses don't need to tick input - dryFrames_ contains "dry" input by the time
    // computeSynthesisBlock() is called
    inline void WetDryEffect_::tick(TonicFrames &frames, const SynthesisContext_ &context ){
      
      // check context to see if we need new frames
      if (context.elapsedFrames == 0 || lastFrameIndex_ != context.elapsedFrames){
        
        // get dry input frames
        input_.tick(dryFrames_, context);
        
        computeSynthesisBlock(context);
        
        // bypass processing - still need to compute block so all generators stay in sync
        bool bypass = bypassGen_.tick(context).value != 0.f;
        if (bypass){
          outputFrames_.copy(dryFrames_);
        }
        else{
          // do not apply dry/wet levels if always wet flag is set
          // offers minor CPU usage optimization
          wetLevelGen_.tick(mixWorkspace_, context);
          outputFrames_ *= mixWorkspace_;
          dryLevelGen_.tick(mixWorkspace_, context);
          dryFrames_ *= mixWorkspace_;
          outputFrames_ += dryFrames_;
        }
        
        lastFrameIndex_ = context.elapsedFrames;
      }
      
      // copy synthesis block to frames passed in
      frames.copy(outputFrames_);
      
#ifdef TONIC_DEBUG
      if(!isfinite(frames(0,0))){
        Tonic::error("Effect_::tick NaN or inf detected.");
      }
#endif
      
    }
    
    inline void WetDryEffect_::tickThrough(TonicFrames & inFrames, TonicFrames & outFrames, const SynthesisContext_ & context){
      
      // Do not check context here, assume each call should produce new output.
      
      dryFrames_.copy(inFrames);
      computeSynthesisBlock(context);
      
      // bypass processing - still need to compute block so all generators stay in sync
      bool bypass = bypassGen_.tick(context).value != 0.f;
      if (bypass){
        outFrames.copy(dryFrames_);
      }
      else {
        wetLevelGen_.tick(mixWorkspace_, context);
        outputFrames_ *= mixWorkspace_;
        dryLevelGen_.tick(mixWorkspace_, context);
        dryFrames_ *= mixWorkspace_;
        outputFrames_ += dryFrames_;
        outFrames.copy(outputFrames_);
      }
            
    }
  }
  
  template<class EffectType, class EffectType_>
  class TemplatedWetDryEffect : public TemplatedGenerator<EffectType_>
  {
      
    public:
      
      // This cast is not safe - up to implementation to ensure that templated EffectType_ is actually an Effect_ subclass
      virtual EffectType & input( Generator input ){
        this->gen()->setInput( input );
        return static_cast<EffectType&>(*this);
      }
      
      void tickThrough( TonicFrames & inFrames, const Tonic_::SynthesisContext_ & context){ // ticks in-place
        this->gen()->tickThrough(inFrames, inFrames, context);
      }
      
      void tickThrough(TonicFrames & inFrames, TonicFrames & outFrames, const Tonic_::SynthesisContext_ & context){
        this->gen()->tickThrough(inFrames, outFrames, context);
      }
      
      void setIsStereoInput( bool isStereoInput ){
        this->gen()->setIsStereoInput(isStereoInput);
      }
    
      TONIC_MAKE_CTRL_GEN_SETTERS(EffectType, bypass, setBypassCtrlGen);
      
      // Defaults to 1.0
      TONIC_MAKE_GEN_SETTERS(EffectType, wetLevel, setWetLevelGen);
      
      // Defaults to 0.0 (full wet)
      TONIC_MAKE_GEN_SETTERS(EffectType, dryLevel, setDryLevelGen);

  };
  
  // signal flow operator - sets lhs as input to rhs
  template<class EffectType, class EffectType_>
  static EffectType operator>>(Generator lhs, TemplatedWetDryEffect<EffectType, EffectType_> rhs){
    return rhs.input( lhs );
  }
}

#endif


