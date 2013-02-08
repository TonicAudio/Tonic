//
//  ___FILENAME___
//  Tonic 
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//___COPYRIGHT___
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

#ifndef __Tonic_____FILEBASENAMEASIDENTIFIER_____
#define __Tonic_____FILEBASENAMEASIDENTIFIER_____

#include <iostream>
#include "Generator.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ___FILEBASENAME____ : public Generator_{
      
    protected:
      
    public:
      ___FILEBASENAME____();
      ~___FILEBASENAME____();
      void tick( TonicFrames& frames);
      
    };
    
    inline void ___FILEBASENAME____::tick( TonicFrames& frames){
      
      
    }
    
  }
  
  class ___FILEBASENAME___ : public TemplatedGenerator<Tonic_::___FILEBASENAME____>{
    
  public:

  };
}

#endif /* defined(__Tonic_____FILEBASENAMEASIDENTIFIER_____) */


