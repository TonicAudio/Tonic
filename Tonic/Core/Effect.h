//
//  Effect.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/20/13.
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

#ifndef __Tonic__Effect__
#define __Tonic__Effect__

#include <iostream>
#include "Generator.h"

namespace Tonic {
  
  namespace Tonic_ {
    
    class Effect_ : public Generator_{
      
    protected:
      
      Generator input_;
      
    public:
      
      void setInput( Generator input );
      
    };
    
    inline void Effect_::setInput( Generator input ){
      input_ = input;
    }
    
  }
  
  template<class EffectType, class EffectType_> class TemplatedEffect : public TemplatedGenerator<EffectType_>{
  
  protected:
    
    Generator input_;
    
  public:
    
    // input should not be a Fixed Value, so no need for other setters
    
    inline EffectType & input( Generator input ){
      dynamic_cast<Tonic_::Effect_*>(this->mGen)->setInput( input );
      return static_cast<EffectType&>(*this);
    }
    
  };

}

#endif /* defined(__Tonic__Effect__) */


