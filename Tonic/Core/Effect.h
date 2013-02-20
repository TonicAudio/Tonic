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
      lockMutex();
      input_ = input;
      unlockMutex();
    }
    
  }
  
  class Effect : public Generator {
    
  public:
    
    //! This cast is not safe - assumes underlying generator is, in fact, an Effect_ subclass
    inline Effect & input( Generator input ){
      static_cast<Tonic_::Effect_*>(mGen)->setInput(input);
      return *this;
    };

  };
  
  template<class GenType> class TemplatedEffect : public Effect{
  protected:
    GenType* gen(){
      return static_cast<GenType*>(mGen);
    }
  public:
    TemplatedEffect(){
      delete mGen;
      mGen = new GenType();
    }
  };
}

#endif /* defined(__Tonic__Effect__) */


