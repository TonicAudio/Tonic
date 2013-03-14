//
//  StereoDelay.h
//  Tonic 
//
//  Created by Morgan Packard on 3/13/13.
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

#ifndef __Tonic__StereoDelay__
#define __Tonic__StereoDelay__

#include "Generator.h"
#include "FixedValue.h"
#include "MonoToStereoPanner.h"
#include "MonoDelay.h"
#include "Adder.h"
#include "Effect.h"

namespace Tonic {
  
  namespace Tonic_ {

    class StereoDelay_ : public Effect_{
      
    protected:
    
      MonoDelay leftDelay;
      MonoDelay rightDelay;
      MonoToStereoPanner leftPanner;
      MonoToStereoPanner rightPanner;
      Generator outputGen;
      
    public:
      StereoDelay_();
      ~StereoDelay_();
      void computeSynthesisBlock( const SynthesisContext_ &context );
      void setInput(Generator);
      
    };
    
    inline void StereoDelay_::computeSynthesisBlock(const SynthesisContext_ &context){
      outputGen.tick(synthesisBlock_, context);
    }
    
    
  }
  
  class StereoDelay : public TemplatedEffect<StereoDelay, Tonic_::StereoDelay_>{
    
  public:

  };
}

#endif /* defined(__Tonic__StereoDelay__) */


