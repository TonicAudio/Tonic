//
//  Filters.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/22/13.
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

#ifndef __Tonic__Filters__
#define __Tonic__Filters__

#include <iostream>
#include "Effect.h"
#include "Biquad.h"
#include "FilterUtils.h"

namespace Tonic {
  
  namespace Tonic_ {
    
    // ================================
    //       Filter Base Class
    // ================================
      
    //! Basic biquad-backed filter Effect_ subclass with inputs for cutoff and Q
    class Filter_ : public Effect_{
        
    protected:
        
        Generator cutoff_;
        Generator Q_;
        
    public:
        
        inline Filter_ & setCutoff( Generator cutoff ){ cutoff_ = cutoff; return *this; };
        inline Filter_ & setQ( Generator Q ){ Q_ = Q; return *this; }
      
    };
      
      
    // ===============================
    //            LPF 12
    // ===============================
      
    //! Butterworth 2-pole LPF
    
  }
  
  
}

#endif /* defined(__Tonic__Filters__) */


