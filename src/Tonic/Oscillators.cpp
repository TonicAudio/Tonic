//
//  Oscillators.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/8/13.
//

#include "Oscillators.h"

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
    
    void TableLookupOsc_::setLookupTable(SampleTable table){
      if (table.channels() != 1){
        error("TableLookupOsc expects lookup table with 1 channel only");
        return;
      }
      
      unsigned int nearestPo2;
      if (!isPowerOf2(table.size()-1, &nearestPo2)){
        
        warning("TableLookUpOsc lookup tables must have a (power-of-two + 1) number of samples (example 2049 or 4097). Resizing to nearest power-of-two + 1");
        
        table.resample(nearestPo2, 1);
        table.resize(nearestPo2+1, 1);
        table.dataPointer()[nearestPo2] = table.dataPointer()[0]; // copy first sample to last
        
      }
      
      lookupTable_ = table;
    }
    
    AngularWave_::AngularWave_() : phaseAccum_(0) {
      
      freqFrames_.resize(kSynthesisBlockSize, 1, 0);
      slopeFrames_.resize(kSynthesisBlockSize, 1, 0);
      
      slopeGen_ = FixedValue(0);
      freqGen_ = FixedValue(440);
      
    }
    
    RectWave_::RectWave_() : phaseAccum_(0) {
      pwmGen_ = FixedValue(0.5);
      freqFrames_.resize(kSynthesisBlockSize, 1 ,0);
      pwmFrames_.resize(kSynthesisBlockSize, 1, 0);
    }

  } // Namespace Tonic_
    
  
  TableLookupOsc & TableLookupOsc::setLookupTable(SampleTable lookupTable){
    gen()->setLookupTable(lookupTable);
    return *this;
  }
  
  // ---------------------------
  
  static string const TONIC_SIN_TABLE = "_TONIC_SIN_TABLE_";
  
  SineWave::SineWave(){
    
    // As soon as the first SineWave is allocated, persistent SampleTable is created. Will stay in memory for program lifetime.
    if (!Tonic_::s_oscTables_.containsObjectNamed(TONIC_SIN_TABLE)){
      
      const unsigned int tableSize = 4096;
      
      SampleTable sineTable = SampleTable(tableSize+1, 1);
      TonicFloat norm = 1.0f / tableSize;
      TonicFloat *data = sineTable.dataPointer();
      for ( unsigned long i=0; i<tableSize+1; i++ ){
        *data++ = sinf( TWO_PI * i * norm );
      }
      
      Tonic_::s_oscTables_.insertObject(TONIC_SIN_TABLE, sineTable);
      
      this->gen()->setLookupTable(sineTable);
    }
    else{
      this->gen()->setLookupTable(Tonic_::s_oscTables_.objectNamed(TONIC_SIN_TABLE));
    }
    
    
  }

  
  
} // Namespace Tonic
