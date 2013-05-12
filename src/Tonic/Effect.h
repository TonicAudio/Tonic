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
      
    protected:
      
      Generator input_;
      
      ControlGenerator bypassGen_;
      
      TonicFrames dryFrames_;
      
      bool isStereoInput_;
      
    public:
      
      Effect_();
      
      void setBypassGenerator( ControlGenerator gen ){
        bypassGen_ = gen;
      };
      
      virtual void tick(TonicFrames &frames, const SynthesisContext_ &context );
      
      virtual void setInput( Generator input ) {
        input_ = input;
      };

      //! set stereo/mono - changes number of channels in dryFrames_
      /*!
          subclasses should call in constructor to determine input channel layout
      */
      virtual void setIsStereoInput( bool stereo );
      
      bool isStereoInput(){ return isStereoInput_; };
      
      //! Apply effect directly to passed in frames (output in-place)
      /*!
          DO NOT mix calls to tick() with calls to tickThrough(). Result is undefined.
      */
      virtual void tickThrough( TonicFrames & inFrames, TonicFrames & outFrames );

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
        
        input_.tick(dryFrames_, context); // get input frames
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
    
    inline void Effect_::tickThrough(TonicFrames & inFrames, TonicFrames & outFrames){

      lockMutex();
      dryFrames_.copy(inFrames);
      computeSynthesisBlock(SynthesisContext_());
      
      // bypass processing - still need to compute block so all generators stay in sync
      bool bypass = bypassGen_.tick(SynthesisContext_()).value != 0.f;
      if (bypass){
        outputFrames_.copy(dryFrames_);
      }
      else{
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
    
    void tickThrough( TonicFrames & inFrames){ // ticks in-place
      this->gen()->tickThrough(inFrames, inFrames);
    }

    
    void tickThrough(TonicFrames & inFrames, TonicFrames & outFrames){
      this->gen()->tickThrough(inFrames, outFrames);
    }
    
    createControlGeneratorSetters(EffectType, bypass, setBypassGenerator);
  
  };
  
  // signal flow operator - sets lhs as input to rhs
  template<class EffectType, class EffectType_>
  static EffectType & operator>>(Generator lhs, TemplatedEffect<EffectType, EffectType_> rhs){
    return rhs.input( lhs );
  }

}

#endif /* defined(__Tonic__Effect__) */


