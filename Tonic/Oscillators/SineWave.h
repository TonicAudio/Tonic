//
//  SineWave.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/8/13.
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

#ifndef __Tonic__SineWave__
#define __Tonic__SineWave__

#include <iostream>
#include "TableLookupOsc.h"

namespace Tonic {
  
  namespace Tonic_ {
    
    class SineWave_ : public TableLookupOsc_{
      
    protected:
      
      static TonicFrames sineTable_;
      TonicFrames & tableReference();
      
    public:
      
      SineWave_();
      void fillTable();
      
    };
    
    inline TonicFrames & SineWave_::tableReference(){ return sineTable_; }
  
  }
  
  class SineWave : public TemplatedGenerator<Tonic_::SineWave_>{
    
  public:
    
    SineWave& freq(Generator freq){
      gen()->setFrequency(freq);
      return *this;
    }
    
    SineWave& freq(float freqArg){
      return freq( FixedValue(freqArg) );
    }
    
  };
}

#endif /* defined(__Tonic__SineWave__) */


