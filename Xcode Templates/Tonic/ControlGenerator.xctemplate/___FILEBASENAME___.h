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

#include "ControlGenerator.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ___FILEBASENAME____ : public ControlGenerator_{
      
    protected:
      TonicFloat computeValue(const SynthesisContext_ & context);
      ControlGeneratorStatus computeStatus(const SynthesisContext_ & context);
      
    public:
      ___FILEBASENAME____();
      ~___FILEBASENAME____();
      
    };
    
  }
  
  class ___FILEBASENAME___ : public TemplatedControlGenerator<Tonic_::___FILEBASENAME____>{
    
  public:

  };
}

#endif /* defined(__Tonic_____FILEBASENAMEASIDENTIFIER_____) */


