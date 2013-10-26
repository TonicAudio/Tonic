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

#include "Generator.h"
#include "FixedValue.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ___FILEBASENAME____ : public Generator_{
      
    protected:
      
    public:
      ___FILEBASENAME____();
      ~___FILEBASENAME____();
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    };
    
    inline void ___FILEBASENAME____::computeSynthesisBlock(const SynthesisContext_ &context){
      
      
    }
    
  }
  
  class ___FILEBASENAME___ : public TemplatedGenerator<Tonic_::___FILEBASENAME____>{
    
  public:

  };
}

#endif /* defined(__Tonic_____FILEBASENAMEASIDENTIFIER_____) */


