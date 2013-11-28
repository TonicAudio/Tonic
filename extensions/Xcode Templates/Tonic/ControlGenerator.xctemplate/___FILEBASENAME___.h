//
//  ___FILENAME___
//  Tonic 
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic_____FILEBASENAMEASIDENTIFIER_____
#define __Tonic_____FILEBASENAMEASIDENTIFIER_____

#include "ControlGenerator.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ___FILEBASENAME____ : public ControlGenerator_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
      
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


