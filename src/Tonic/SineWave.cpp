//
//  SineWave.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/8/13.

//

#include "SineWave.h"

namespace Tonic {
  
  static string const TONIC_SIN_TABLE = "_TONIC_SIN_TABLE_";
  
  SineWave::SineWave(){
    
    // As soon as the first SineWave is allocated, persistent SampleTable is created. Will stay in memory for program lifetime.
    SampleTable sineTable;
    if (!SharedSampleTables().getSampleTable(TONIC_SIN_TABLE, &sineTable)){
      
      const unsigned int tableSize = 4096;
      
      sineTable = SampleTable(tableSize, 1);
      TonicFloat norm = 1.0f / tableSize;
      TonicFloat *data = sineTable.dataPointer();
      for ( unsigned long i=0; i<=tableSize; i++ ){
        *data++ = sinf( TWO_PI * i * norm );
      }
      
      SharedSampleTables().registerSampleTable(TONIC_SIN_TABLE, sineTable);
    }
    
    this->gen()->setSampleTable(sineTable);
    
  }
  
} // Namespace Tonic
