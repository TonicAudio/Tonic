//
//  Noise.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/7/13.
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

#ifndef __Tonic__Noise__
#define __Tonic__Noise__

#include <iostream>
#include "Generator.h"

namespace Tonic {
  
  namespace Tonic_ {
    
    class Noise_ : public Generator_{
      
    protected:
      
      bool stereo_;
      
    public:
      Noise_();
      ~Noise_();
      void computeSynthesisBlock( const SynthesisContext & context );
      inline void setStereo(bool stereo) { stereo_ = stereo; };
      
    };
    
    inline void Noise_::computeSynthesisBlock( const SynthesisContext & context ){
      TonicFloat* fdata = &synthesisBlock_[0];
      if (stereo_){
        for (unsigned int i=0; i<synthesisBlock_.size(); i++){
          *fdata++ = randomSample();
        }
      }
      else{
        unsigned int stride = synthesisBlock_.channels();
        for (unsigned int i=0; i<synthesisBlock_.frames(); i++){
          *fdata = randomSample();
          fdata += stride;
        }
        
        synthesisBlock_.fillChannels();
      }
    }
    
  }
  
  class Noise : public TemplatedGenerator<Tonic_::Noise_>{

  public:
    Noise & stereo(bool stereo);
    
  };
}

#endif /* defined(__Tonic__Noise__) */


