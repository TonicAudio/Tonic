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
      
      virtual inline void setInput( Generator input ) {
        input_ = input;
        if(input.isStereo() && !isStereo()){
          setIsStereo(input.isStereo());
        }
      };
      
    };
  }
  
  template<class EffectType, class EffectType_>
  class TemplatedEffect : public TemplatedGenerator<EffectType_>{
  
  protected:
    
    Generator input_;
    
  public:
        
    // This cast is not safe - up to implementation to ensure that templated EffectType_ is actually an Effect_ subclass
    inline EffectType & input( Generator input ){
      this->gen()->setInput( input );
      return static_cast<EffectType&>(*this);
    }
    
  };
  
  // signal flow operator - sets lhs as input to rhs
  template<class EffectType, class EffectType_>
  static EffectType & operator>>(Generator lhs, TemplatedEffect<EffectType, EffectType_> rhs){
    return rhs.input( lhs );
  }

}

#endif /* defined(__Tonic__Effect__) */


