//
//  Effect.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/20/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__Effect__
#define __Tonic__Effect__

#include <iostream>
#include "Generator.h"

namespace Tonic {
  
  namespace Tonic_ {
    
    class Effect_ : public Generator_{
      
    private:
      
      ControlGenerator bypassGen_;
      Generator  dryLevelGen_;
      Generator  wetLevelGen_;
      TonicFrames mixWorkspace_;
      bool isAlwaysWet_;
      bool isStereoInput_;
      
    protected:
      
      Generator input_;
      TonicFrames dryFrames_;
      
    public:
      
      Effect_();
            
      void setBypassCtrlGen( ControlGenerator gen ){ bypassGen_ = gen; };
      void setDryLevelGen( Generator gen ){ dryLevelGen_ = gen; };
      void setWetLevelGen( Generator gen ){ wetLevelGen_ = gen; };

      virtual void setInput( Generator input ) { input_ = input; };
      
      //! set to true to disable dry/wet (always fully wet)
      void setIsAlwaysWet( bool isAlwaysWet ) { isAlwaysWet_ = isAlwaysWet; };
      
      //! set stereo/mono - changes number of channels in dryFrames_
      /*!
          subclasses should call in constructor to determine input channel layout
      */
      virtual void setIsStereoInput( bool stereo );
      bool isStereoInput(){ return isStereoInput_; };
      
      // --- Tick methods ---
      
      virtual void tick(TonicFrames &frames, const SynthesisContext_ &context );
      
      //! Apply effect directly to passed in frames (output in-place)
      /*!
          DO NOT mix calls to tick() with calls to tickThrough(). Result is undefined.
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
    // subclasses don't need to tick input - dryFrames_ contains "dry" input
    inline void Effect_::tick(TonicFrames &frames, const SynthesisContext_ &context ){
      
      // check context to see if we need new frames
      if (context.elapsedFrames == 0 || lastFrameIndex_ != context.elapsedFrames){
        
        lockMutex();
        
        input_.tick(dryFrames_, context); // get input frames
        
        computeSynthesisBlock(context);

        // bypass processing - still need to compute block so all generators stay in sync
        bool bypass = bypassGen_.tick(context).value != 0.f;
        if (bypass){
          outputFrames_.copy(dryFrames_);
        }
        else if (!isAlwaysWet_){
          wetLevelGen_.tick(mixWorkspace_, context);
          outputFrames_ *= mixWorkspace_;
          dryLevelGen_.tick(mixWorkspace_, context);
          dryFrames_ *= mixWorkspace_;
          outputFrames_ += dryFrames_;
        }
        
        unlockMutex();
        
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

        // Do not check context here, assume each call should process
      
        lockMutex();

        dryFrames_.copy(inFrames);
        computeSynthesisBlock(context);
        
        // bypass processing - still need to compute block so all generators stay in sync
        bool bypass = bypassGen_.tick(context).value != 0.f;
        if (bypass){
          outFrames.copy(dryFrames_);
        }
        else{
          
          if (!isAlwaysWet_){
            wetLevelGen_.tick(mixWorkspace_, context);
            outputFrames_ *= mixWorkspace_;
            dryLevelGen_.tick(mixWorkspace_, context);
            dryFrames_ *= mixWorkspace_;
            outputFrames_ += dryFrames_;
          }
          
          outFrames.copy(outputFrames_);
        }


        unlockMutex();
      
    }
  }
  
  template<class EffectType, class EffectType_>
  class TemplatedEffect : public TemplatedGenerator<EffectType_>{
  
  protected:
    
    Generator input_;
    
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
      this->gen()->lockMutex();
      this->gen()->setIsStereoInput(isStereoInput);
      this->gen()->unlockMutex();
    }
    
    createControlGeneratorSetters(EffectType, bypass, setBypassCtrlGen);
    
    // Defaults to 1.0
    createGeneratorSetters(EffectType, wetLevel, setWetLevelGen);
    
    // Defaults to 0.0 (full wet)
    createGeneratorSetters(EffectType, dryLevel, setDryLevelGen);
  };
  
  // signal flow operator - sets lhs as input to rhs
  template<class EffectType, class EffectType_>
  static EffectType operator>>(Generator lhs, TemplatedEffect<EffectType, EffectType_> rhs){
    return rhs.input( lhs );
  }

}

#endif /* defined(__Tonic__Effect__) */


