//
//  TableLookupOsc.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/8/13.

//

#include "TableLookupOsc.h"

namespace Tonic {
  
  namespace Tonic_{
  
    TableLookupOsc_::TableLookupOsc_() :
      phase_(0.0)
    {
      modFrames_.resize(kSynthesisBlockSize, 1);
      lookupTable_ = SampleTable(kSynthesisBlockSize,1);
    }
    
    void TableLookupOsc_::reset(){
      phase_ = 0.0f;
    }

  } // Namespace Tonic_
    
  
  TableLookupOsc & TableLookupOsc::setLookupTable(SampleTable lookupTable){
    
    if (lookupTable.channels() != 1){
      error("TableLookupOsc expects lookup table with 1 channel only");
      return *this;
    }
    
    unsigned int lowerPo2;
    if (!isPowerOf2(lookupTable.frames(), &lowerPo2)){
      
      warning("TableLookUpOsc lookup tables must have a power-of-two number of frames. Resizing to next lowest power-of-two");
      
      lookupTable.resize(lowerPo2, 1);
      
    }
    
    gen()->lockMutex();
    gen()->setLookupTable(lookupTable);
    gen()->unlockMutex();
    return *this;
  }
  
  
} // Namespace Tonic
