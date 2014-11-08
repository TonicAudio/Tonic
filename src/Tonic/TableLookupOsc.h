//
//  TableLookupOsc.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/8/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_TABLELOOKUPOSC_H
#define TONIC_TABLELOOKUPOSC_H

#include "Generator.h"
#include "SampleTable.h"

namespace Tonic {
  
  namespace Tonic_ {
    
    // Registry for all static oscillator lookup table data
    TonicDictionary<SampleTable> * s_oscillatorTables();
    
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
      
      SampleTable lookupTable_;
      
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
      }
      
      //! set sample table for lookup. MUST BE POWER OF 2 IN LENGTH
      void setLookupTable( SampleTable table );

    };
    
    inline void TableLookupOsc_::computeSynthesisBlock( const SynthesisContext_ & context ){
      
      // Update the frequency data
      frequencyGenerator_.tick(modFrames_, context);
      
      unsigned long tableSize = lookupTable_.size()-1;
      
      const TonicFloat rateConstant = (TonicFloat)tableSize / Tonic::sampleRate();
      
      TonicFloat *samples = &outputFrames_[0];
      TonicFloat *rateBuffer = &modFrames_[0];
      TonicFloat *tableData = lookupTable_.dataPointer();
      
      // R. Hoelderich style fast phasor.
      
      FastPhasor sd;
      
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
      
      TonicInt32 offs;
      TonicInt32 msbi = sd.i[1];
      
      double frac;
      double ps = phase_ + BIT32DECPT;
      
      TonicFloat *tAddr, f1, f2;
      
      for ( unsigned int i=0; i<kSynthesisBlockSize; i++ ) {
        
        sd.d = ps;
        ps += *rateBuffer++;
        offs = sd.i[1] & (tableSize-1);
        tAddr = tableData + offs;
        sd.i[1] = msbi;
        frac = sd.d - BIT32DECPT;
        f1 = tAddr[0];
        f2 = tAddr[1];
        
        *samples++ = f1 + frac * (f2 - f1);
      }
      
      sd.d = BIT32DECPT * tableSize;
      msbi = sd.i[1];
      sd.d = ps + (BIT32DECPT * tableSize - BIT32DECPT);
      sd.i[1] = msbi;
      phase_ = sd.d - BIT32DECPT * tableSize;
      
    }

  }
  
  class TableLookupOsc : public TemplatedGenerator<Tonic_::TableLookupOsc_>{
    
    public:
    
      TableLookupOsc & setLookupTable( SampleTable lookupTable );
    
      TONIC_MAKE_GEN_SETTERS(TableLookupOsc, freq, setFrequency);
  };

}

#endif


