//
//  TableLookupOsc.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/8/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "TableLookupOsc.h"

namespace Tonic { namespace Tonic_{
  
  TableLookupOsc_::TableLookupOsc_() :
    phase_(0.0)
  {
    modFrames.resize(kSynthesisBlockSize, 1);
  }
  
  TableLookupOsc_::~TableLookupOsc_(){
  
  }
  
  void TableLookupOsc_::reset(){
    phase_ = 0.0f;
  }
  
  inline void TableLookupOsc_::computeSynthesisBlock( const SynthesisContext_ & context ){
    
    // Update the frequency data
    lockMutex();
    frequencyGenerator.tick(modFrames, context);
    unlockMutex();
    
    static const TonicFloat rateConstant = (TonicFloat)TABLE_SIZE / Tonic::sampleRate();
    const unsigned int nFrames = synthesisBlock_.frames();
    const unsigned int stride = synthesisBlock_.channels();
    
    TonicFloat *samples = &synthesisBlock_[0];
    TonicFloat *rateBuffer = &modFrames[0];
    TonicFloat *tableData = &(tableReference())[0];
    
    ShiftedDouble sd;
    
    // pre-multiply rate constant for speed
#ifdef USE_APPLE_ACCELERATE
    vDSP_vsmul(rateBuffer, 1, &rateConstant, rateBuffer, 1, nFrames);
#else
    for (unsigned int i=0; i<nFrames; i++){
      *rateBuffer++ *= rateConstant;
    }
    rateBuffer = &modFrames[0];
#endif
    
    sd.d = BIT32DECPT;
    int32_t offs, msbi = sd.i[1];
    double ps = phase_ + BIT32DECPT;
    
    TonicFloat *tAddr, f1, f2, frac;
    
    for ( unsigned int i=0; i<synthesisBlock_.frames(); i++ ) {
      
      sd.d = ps;
      ps += *rateBuffer++;
      offs = sd.i[1] & (TABLE_SIZE-1);
      tAddr = tableData + offs;
      sd.i[1] = msbi;
      frac = sd.d - BIT32DECPT;
      f1 = tAddr[0];
      f2 = tAddr[1];
      
      *samples = f1 + frac * (f2 - f1);
      samples += stride;
      
    }
    
    sd.d = BIT32DECPT * TABLE_SIZE;
    msbi = sd.i[1];
    sd.d = ps + (BIT32DECPT * TABLE_SIZE - BIT32DECPT);
    sd.i[1] = msbi;
    phase_ = sd.d - BIT32DECPT * TABLE_SIZE;
    
  }

} // Namespace Tonic_
  
  
  
} // Namespace Tonic
