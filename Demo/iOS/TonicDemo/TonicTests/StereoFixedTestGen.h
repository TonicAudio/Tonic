//
//  StereoFixedTestGen.h
//  Tonic 
//
//  Created by Nick Donaldson on 3/9/13.
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

#ifndef __Tonic__StereoFixedTestGen__
#define __Tonic__StereoFixedTestGen__

#include "Generator.h"

namespace Tonic {
  
  namespace Tonic_ {
    
    class StereoFixedTestGen_ : public Generator_{
      
    protected:
      
      void computeSynthesisBlock( const SynthesisContext_ & context);
      
      float lVal_;
      float rVal_;
      
    public:
      StereoFixedTestGen_();
      ~StereoFixedTestGen_();
      
      void setLVal(float l){ lVal_ = l; };
      void setRVal(float r){ rVal_ = r; };
      
    };
    
    inline void StereoFixedTestGen_::computeSynthesisBlock( const SynthesisContext_ & context)
    {
      float* buffStart = &synthesisBlock_[0];
      
        
      #ifdef USE_APPLE_ACCELERATE
        
      vDSP_vfill( &lVal_ , buffStart, 2, synthesisBlock_.frames());
      vDSP_vfill( &rVal_ , buffStart+1, 2, synthesisBlock_.frames());
      
      #else
      
      for (unsigned int i=0; i<synthesisBlock_.frames(); i++){
        *buffStart++ = lVal_;
        *buffStart++ = rVal_;
      }
      
      #endif
      
    }
    
  }
  
  class StereoFixedTestGen : public TemplatedGenerator<Tonic_::StereoFixedTestGen_>{
    
  public:
    
    StereoFixedTestGen(float left, float right)
    {
      gen()->setLVal(left);
      gen()->setRVal(right);
      gen()->setIsStereo(true);
    }

  };
}

#endif /* defined(__Tonic__StereoFixedTestGen__) */


