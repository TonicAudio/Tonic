//
//  BLEPOscillator.cpp
//  Tonic
//
//  Created by Nick D on 7/21/13.
//

#include "BLEPOscillator.h"

namespace Tonic { namespace Tonic_{
  
  TonicFloat * BLEPOscillator_::minBLEP_ = NULL;
  int BLEPOscillator_::minBLEPlength_ = 0;
  
  BLEPOscillator_::BLEPOscillator_() :
    phase_(0),
    accum_(0),
    lBuffer_(0),
    iBuffer_(0),
    nInit_(0),
    ringBuf_(NULL)
  {
      // Generate static minBLEP lookup
      // TODO: Maybe do this as global constructor to avoid halting while this is computed on creation of first instance
      // (it's sort of slow)
      if (NULL == minBLEP_)
      {
        minBLEP_ = GenerateMinBLEP(TONIC_MINBLEP_ZEROCROSSINGS, TONIC_MINBLEP_OVERSAMPLING);
        minBLEPlength_ = TONIC_MINBLEP_ZEROCROSSINGS * TONIC_MINBLEP_OVERSAMPLING * 2;
      }
    
      lBuffer_ = minBLEPlength_/TONIC_MINBLEP_OVERSAMPLING;
      ringBuf_ = new TonicFloat[lBuffer_+1];
      memset(ringBuf_, 0, (lBuffer_+1)*sizeof(TonicFloat));
    
      freqFrames_.resize(kSynthesisBlockSize, 1, 0);
  }
  
  BLEPOscillator_::~BLEPOscillator_()
  {
    if (ringBuf_)
    {
      delete [] ringBuf_;
      ringBuf_ = NULL;
    }
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
