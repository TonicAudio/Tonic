//
//  Generator.cpp
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/23/13.
//
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

#include "Generator.h"
#include "Multiplier.h"

namespace Tonic{ namespace Tonic_{
  
  Generator_::Generator_(){
    pthread_mutex_init(&genMutex_, NULL);
  }

  Generator_::~Generator_(){
    pthread_mutex_destroy(&genMutex_);
  }

}}
