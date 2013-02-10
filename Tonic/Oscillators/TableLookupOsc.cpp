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
  
  inline void TableLookupOsc_::tick( TonicFrames& frames){
    
    // Update the frequency data
    lockMutex();
    frequencyGenerator.tick(modFrames);
    unlockMutex();
    
    static const TonicFloat rateConstant = (TonicFloat)TABLE_SIZE / Tonic::sampleRate();
    const unsigned int nFrames = frames.frames();
    const unsigned int stride = frames.channels();
    
    TonicFloat *samples = &frames[0];
    TonicFloat *rateBuffer = &modFrames[0];
    TonicFloat *tableData = &(tableReference())[0];
    
    LookupFudge lf;
    
    // pre-multiply rate constant for speed
#ifdef USE_APPLE_ACCELERATE
    vDSP_vsmul(rateBuffer, 1, &rateConstant, rateBuffer, 1, nFrames);
#else
    for (unsigned int i=0; i<modFrames.frames(); i++){
      *rateBuffer++ *= rateConstant;
    }
    rateBuffer = &modFrames[0];
#endif
    
    lf.lf_d = UNITBIT32;
    int32_t nhi = lf.lf_i[1];
    double dp = phase_ + UNITBIT32;
    
    TonicFloat *tAddr, f1, f2, frac;
    
    for ( unsigned int i=0; i<frames.frames(); i++ ) {
      
      lf.lf_d = dp;
      dp += *rateBuffer++;
      tAddr = tableData + (lf.lf_i[1] & (TABLE_SIZE-1));
      lf.lf_i[1] = nhi;
      frac = lf.lf_d - UNITBIT32;
      f1 = tAddr[0];
      f2 = tAddr[1];
      
      // fill all channels of the interleaved output
      // it's up to the caller to request the right number of channels, since a sine wave is always mono
      *samples = f1 + frac * (f2 - f1);
      samples += stride;
      
    }
    
    lf.lf_d = UNITBIT32 * TABLE_SIZE;
    nhi = lf.lf_i[1];
    lf.lf_d = dp + (UNITBIT32 * TABLE_SIZE - UNITBIT32);
    lf.lf_i[1] = nhi;
    phase_ = lf.lf_d - UNITBIT32 * TABLE_SIZE;
    
    // mono source, so copy channels if necessary
    frames.fillChannels();
    
  }

  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
