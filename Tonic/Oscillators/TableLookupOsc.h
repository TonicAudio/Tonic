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
      
      TonicFloat time_;
      TonicFloat rate_;
      unsigned int iIndex_;
      TonicFloat alpha_;
      
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
      
      // can use vDSP_vtabi to perform lookup, with some conditioning first
      #ifdef USE_APPLE_ACCELERATE

      // pre-multiply rate constant 
      vDSP_vsmul(rateBuffer, 1, &rateConstant, rateBuffer, 1, modFrames.frames());
      
      // compute wrapped time values, can use modFrames as workspace
      for (unsigned int i=0; i<modFrames.frames(); i++){
        time_ += rateBuffer[i];
        
        while ( time_ < 0.0 )
          time_ += TABLE_SIZE;
        while ( time_ >= TABLE_SIZE )
          time_ -= TABLE_SIZE;
        
        rateBuffer[i] = time_;
      }
      
      // perform table lookup
      static float tScale = 1.0f;
      static float tOffset = 0.0f;
      vDSP_vtabi(rateBuffer, 1, &tScale, &tOffset, tableData, TABLE_SIZE, samples, stride, frames.frames());
      
      #else

      TonicFloat tmp = 0.0;

      // pre-multiply rate constant for speed
      for (unsigned int i=0; i<modFrames.frames(); i++){
        *rateBuffer++ *= rateConstant;
      }
      rateBuffer = &modFrames[0];
      
      
      for ( unsigned int i=0; i<frames.frames(); i++ ) {
        
        // This can be optimized by pre-multiplying to get rate
        // SineWave_::setFrequency(*(freqBuffer++));
        
        // Check limits of time address ... if necessary, recalculate modulo
        // TABLE_SIZE.
        while ( time_ < 0.0 )
          time_ += TABLE_SIZE;
        while ( time_ >= TABLE_SIZE )
          time_ -= TABLE_SIZE;
        
        iIndex_ = (unsigned int) time_;
        alpha_ = time_ - (TonicFloat)iIndex_;
        tmp = tableData[ iIndex_ ];
        tmp += ( alpha_ * ( tableData[ iIndex_ + 1 ] - tmp ) );
        
        // fill all channels of the interleaved output
        // it's up to the caller to request the right number of channels, since a sine wave is always mono
        *samples = tmp;
        samples += stride;
        
        // Increment time, which can be negative.
        // Directly add the rate, don't need to dive into function call
        time_ += *rateBuffer++;
      }
      #endif
      
      // mono source, so copy channels if necessary
      frames.fillChannels();

    }
  
  }

}

#endif /* defined(__Tonic__TableLookupOsc__) */


