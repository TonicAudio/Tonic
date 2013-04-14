//
//  SineWave.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/8/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__SineWave__
#define __Tonic__SineWave__

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
    
    createGeneratorSetters(SineWave, freq, setFrequency)

  };
}

#endif /* defined(__Tonic__SineWave__) */


