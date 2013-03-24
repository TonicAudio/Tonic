//
//  Compressor.h
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

namespace Tonic {
  
  namespace Tonic_ {

    class Compressor_ : public Effect_{
      
    protected:
      
      enum EnvelopeState {
        ES_ATTACK = 0,
        ES_RELEASE = 1
      };
      
      // Can be overridden for sidechaining
      Generator amplitudeInput_;
      
      ControlGenerator attackGen_;
      ControlGenerator releaseGen_;
      ControlGenerator threshGen_;
      ControlGenerator ratioGen_;
      ControlGenerator lookaheadGen_;
      
      DelayLine lookaheadDelayLine_;
      
      TonicFrames ampInputFrames_;
      
      EnvelopeState envState_;
      
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
      void setPreDelay( ControlGenerator gen ) { lookaheadGen_ = gen; };
      
    };
    
    inline void Compressor_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      
    }
    
  }
  
  class Compressor : public TemplatedEffect<Compressor, Tonic_::Compressor_>{
    
  public:

  };
}

#endif /* defined(__Tonic__Compressor__) */


