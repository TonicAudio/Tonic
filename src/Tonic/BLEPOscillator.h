//
//  BLEPOscillator.h
//  Tonic 
//
//  Created by Nick D on 7/21/13.
//
// See LICENSE.txt for license and usage information.
//

/*****************************************************
 *
 *  Abstract base Generator_ class for bandlimited
 *  waveform generation using minBLEP technique.
 *
 *  Sources here:
 *
 *  http://www.experimentalscene.com/articles/minbleps.php
 *  http://www.musicdsp.org/showArchiveComment.php?ArchiveID=112
 *
 *****************************************************/


#ifndef __Tonic__BLEPOscillator__
#define __Tonic__BLEPOscillator__

#include "Generator.h"
#include "DSPUtils.h"

namespace Tonic {
  
  namespace Tonic_ {

    class BLEPOscillator_ : public Generator_{
      
    protected:
      
      // Input generators
      Generator freqGen_;
      TonicFrames freqFrames_;
      
      // TODO: Hardsync?
      
      // Lookup table
      static TonicFloat * minBLEP_;
      static int minBLEPlength_;
      
      // phase accumulator and fast phasor
      FastPhasor fastPhasor_;
      double phase_;
      
      // ring buffer and accumulator
      TonicFloat * ringBuf_;
      TonicFloat accum_;
      int lBuffer_; // ring buffer length
      int iBuffer_; // current index
            
      // add a BLEP to the ring buffer at the specified offset
      inline void addBLEP(TonicFloat offset, TonicFloat scale)
      {
        
      }
  
    public:

      BLEPOscillator_();
      ~BLEPOscillator_();
      
      void setFreqGen(Generator gen) { freqGen_ = gen; };
      
    };
    
  }
}

#endif /* defined(__Tonic__BLEPOscillator__) */


