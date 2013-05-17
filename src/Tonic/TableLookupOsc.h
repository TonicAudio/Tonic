//
//  TableLookupOsc.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/8/13.

//
// See LICENSE.txt for license and usage information.
//


/*
 This class is heavily borrowed from the PD implementation of a table-lookup oscillator.
 Necessary attribution/license TBD.
*/

#ifndef __Tonic__TableLookupOsc__
#define __Tonic__TableLookupOsc__

#include "Generator.h"

namespace Tonic {
  
  namespace Tonic_ {
    
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
      
      Generator frequencyGenerator_;
      TonicFrames modFrames_;
      
      void computeSynthesisBlock( const SynthesisContext_ & context );
      
    public:
      
      TableLookupOsc_();

      //! Clear output and reset time pointer to zero.
      void reset( void );
      
      //! Set frequency generator input
      void setFrequency( Generator genArg){
        frequencyGenerator_ = genArg;
      };

    };
    
    inline void TableLookupOsc_::computeSynthesisBlock( const SynthesisContext_ & context ){
      
      // Update the frequency data
      frequencyGenerator_.tick(modFrames_, context);
      
      const TonicFloat rateConstant = (TonicFloat)TABLE_SIZE / Tonic::sampleRate();
      
      TonicFloat *samples = &outputFrames_[0];
      TonicFloat *rateBuffer = &modFrames_[0];
      TonicFloat *tableData = &(tableReference())[0];
      
      ShiftedDouble sd;
      
      // pre-multiply rate constant for speed
#ifdef USE_APPLE_ACCELERATE
      vDSP_vsmul(rateBuffer, 1, &rateConstant, rateBuffer, 1, kSynthesisBlockSize);
#else
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
        *rateBuffer++ *= rateConstant;
      }
      rateBuffer = &modFrames_[0];
#endif
      
      sd.d = BIT32DECPT;
      TonicInt32 offs, msbi = sd.i[1];
      double ps = phase_ + BIT32DECPT;
      
      TonicFloat *tAddr, f1, f2, frac;
      
      for ( unsigned int i=0; i<kSynthesisBlockSize; i++ ) {
        
        sd.d = ps;
        ps += *rateBuffer++;
        offs = sd.i[1] & (TABLE_SIZE-1);
        tAddr = tableData + offs;
        sd.i[1] = msbi;
        frac = sd.d - BIT32DECPT;
        f1 = tAddr[0];
        f2 = tAddr[1];
        
        *samples++ = f1 + frac * (f2 - f1);
      }
      
      sd.d = BIT32DECPT * TABLE_SIZE;
      msbi = sd.i[1];
      sd.d = ps + (BIT32DECPT * TABLE_SIZE - BIT32DECPT);
      sd.i[1] = msbi;
      phase_ = sd.d - BIT32DECPT * TABLE_SIZE;
      
    }

  }

}

#endif /* defined(__Tonic__TableLookupOsc__) */


