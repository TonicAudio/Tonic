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
      
      // Can be overridden for sidechaining
      Generator amplitudeInput_;
      
      ControlGenerator attackGen_;
      ControlGenerator releaseGen_;
      ControlGenerator threshGen_;
      ControlGenerator ratioGen_;
      ControlGenerator preDelayGen_;
      
      DelayLine preDelayLine_;
      
    public:
      Compressor_();
      ~Compressor_();
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    };
    
    inline void Compressor_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      
    }
    
  }
  
  class Compressor : public TemplatedEffect<Compressor, Tonic_::Compressor_>{
    
  public:

  };
}

#endif /* defined(__Tonic__Compressor__) */


