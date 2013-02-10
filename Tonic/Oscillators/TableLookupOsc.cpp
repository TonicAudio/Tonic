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
    
    // can use vDSP_vtabi to perform lookup, with some conditioning first
#ifdef USE_APPLE_ACCELERATE
    
    // pre-multiply rate constant
    vDSP_vsmul(rateBuffer, 1, &rateConstant, rateBuffer, 1, nFrames);
    
    // compute wrapped time values, can use modFrames as workspace
    for (unsigned int i=0; i<nFrames; i++){
      phase_ += *rateBuffer;
      
      while ( phase_ < 0.0 )
        phase_ += TABLE_SIZE;
      while ( phase_ >= TABLE_SIZE )
        phase_ -= TABLE_SIZE;
      
      *rateBuffer++ = phase_;
    }
    rateBuffer = &modFrames[0];
    
    // perform table lookup
    static float tScale = 1.0f;
    static float tOffset = 0.0f;
    vDSP_vtabi(rateBuffer, 1, &tScale, &tOffset, tableData, TABLE_SIZE, samples, stride, nFrames);
    
#else
    
    TonicFloat tmp = 0.0, alpha_;
    unsigned int iIndex_;
    
    // pre-multiply rate constant for speed
    for (unsigned int i=0; i<nFrames; i++){
      *rateBuffer++ *= rateConstant;
    }
    rateBuffer = &modFrames[0];
    
    
    for ( unsigned int i=0; i<nFrames; i++ ) {
      
      // This can be optimized by pre-multiplying to get rate
      // SineWave_::setFrequency(*(freqBuffer++));
      
      // Check limits of time address ... if necessary, recalculate modulo
      // TABLE_SIZE.
      while ( phase_ < 0.0 )
        phase_ += TABLE_SIZE;
      while ( phase_ >= TABLE_SIZE )
        phase_ -= TABLE_SIZE;
      
      iIndex_ = (unsigned int) phase_;
      alpha_ = phase_ - (TonicFloat)iIndex_;
      tmp = tableData[ iIndex_ ];
      tmp += ( alpha_ * ( tableData[ iIndex_ + 1 ] - tmp ) );
      
      *samples = tmp;
      samples += stride;
      
      // Increment time, which can be negative.
      // Directly add the rate, don't need to dive into function call
      phase_ += *rateBuffer++;
    }
#endif
    
    // mono source, so copy channels if necessary
    frames.fillChannels();
    
  }

  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
