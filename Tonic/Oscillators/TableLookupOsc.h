//
//  TableLookupOsc.h
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

/*
 This class is heavily borrowed from the STK implementation of a table-lookup oscillator.
 Much thanks to Perry Cook and Gary Scavone.
 https://ccrma.stanford.edu/software/stk/
 */

#ifndef __Tonic__TableLookupOsc__
#define __Tonic__TableLookupOsc__

#include <iostream>
#include "Generator.h"
#include "FixedValue.h"

#define UNITBIT32 1572864.  /* 3*2^19; bit 32 has place value 1 */

namespace Tonic {
  
  namespace Tonic_ {
    
    // For fast computation of int/fract
    // Implementation similar to pd
    union LookupFudge {
      double lf_d;
      uint32_t lf_i[2];
    };
    
    const unsigned long TABLE_SIZE = 2048;
    
    class TableLookupOsc_ : public Generator_{
      
      
      //------------------------------------
      //
      // Many of the original STK methods are not applicable in our use case
      // (direct phase/freq manipulation) and have been removed for optimization.
      // In the future, phase inputs may be added, but will be optimized and made
      // Tonic-friendly for universal routing.
      //
      //-----------------------------------
      
    protected:

      //! Subclasses MUST override to provide pointer to table
      /*
        Necessary to maintain efficient memory usage by only
        using one set of static wavetable data per subclss, regardless
        of number of instances.
       
        Subclasses should return static member variable.
      */
      virtual TonicFrames & tableReference() = 0;
      
      //! Subclasses implement this to fill the wavetable
      /*
          Subclasses should ALWAYS call this method from their constructor!
      */
      virtual void fillTable() = 0;
      
      double phase_;
      LookupFudge lf;
      
      Generator frequencyGenerator;
      TonicFrames modFrames;
      
    public:
      
      TableLookupOsc_();
      ~TableLookupOsc_();

      
      void tick( TonicFrames& frames);
      
      //! Clear output and reset time pointer to zero.
      void reset( void );
      
      //! Set frequency generator input
      void setFrequency( Generator & genArg){
        // need to lock mutex so we don't replace this in the middle of a tick
        lockMutex();
        frequencyGenerator = genArg;
        unlockMutex();
      };

      
    };
    
<<<<<<< HEAD
=======
    inline void TableLookupOsc_::tick( TonicFrames& frames){
      
      // Update the frequency data
      lockMutex();
      frequencyGenerator.tick(modFrames);
      unlockMutex();
      
      unsigned int stride = frames.channels();
      TonicFloat *samples = &frames[0];
      TonicFloat *rateBuffer = &modFrames[0];
      TonicFloat *tableData = &(tableReference())[0];
      static const TonicFloat rateConstant = (TonicFloat)TABLE_SIZE / Tonic::sampleRate();
      

      // pre-multiply rate constant for speed
      #ifdef USE_APPLE_ACCELERATE
      vDSP_vsmul(rateBuffer, 1, &rateConstant, rateBuffer, 1, modFrames.frames());
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
  
>>>>>>> Fudged bit manipulation for table lookup
  }

}

#endif /* defined(__Tonic__TableLookupOsc__) */


