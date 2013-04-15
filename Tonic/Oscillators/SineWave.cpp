//
//  SineWave.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/8/13.

//

#include "SineWave.h"

namespace Tonic { namespace Tonic_{
  
  TonicFrames SineWave_::sineTable_;
  
  SineWave_::SineWave_(){
    fillTable();
  }
  
  void SineWave_::fillTable(){
    
    TonicFrames & table = tableReference();
    if (table.empty()){
      table.resize( TABLE_SIZE + 1, 1 );
      TonicFloat temp = 1.0 / TABLE_SIZE;
      for ( unsigned long i=0; i<=TABLE_SIZE; i++ )
        table[i] = sinf( TWO_PI * i * temp );
    }
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
