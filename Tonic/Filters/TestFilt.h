//
//  TestFilt.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/20/13.
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

#ifndef __Tonic__TestFilt__
#define __Tonic__TestFilt__

#include <iostream>
#include "Biquad.h"

namespace Tonic {
  
  namespace Tonic_ {
    
    class TestFilt_ : public Biquad_{
      
    protected:
      
    public:
      TestFilt_();
      ~TestFilt_();
    };
    
    
  }
  
  class TestFilt : public TemplatedEffect<Tonic_::TestFilt_>{
    
  public:

  };
}

#endif /* defined(__Tonic__TestFilt__) */


