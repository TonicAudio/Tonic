//
//  ControlConditioner.h
//  TonicDemo
//
//  Created by Morgan Packard on 3/4/13.

//
// See LICENSE.txt for license and usage information.
//

#ifndef __TonicDemo__ControlConditioner__
#define __TonicDemo__ControlConditioner__


#include <vector>
#include "ControlGenerator.h"
#include "ControlValue.h"

using std::vector;

namespace Tonic {
  
  namespace Tonic_ {
    
    class ControlConditioner_ : public ControlGenerator_{
      
    protected:
      
      ControlGenerator input_;
      
    public:
      
      void input( ControlGenerator input );
      
    };
    
  }
  
  // The Tonic:: component of the controlconditioner (the smart pointer wrapper) must subclass the
  // following template, in the form <class Wrapper, class PrivateControlGenerator>
  
  template<class ControlConditionerType, class ControlConditionerType_>
  class TemplatedControlConditioner : public TemplatedControlGenerator<ControlConditionerType_>{
    
  public:
    
    ControlConditionerType input( ControlGenerator input ){
      // why doesn't this compile without 'this' ?
      this->gen()->input( input );
      return static_cast<ControlConditionerType&>(*this);
    }
    
    ControlConditionerType input( float inputVal ){
      return input( ControlValue(inputVal) );
    }
    
  };
  
    // signal flow operator - sets lhs as input to rhs
  template<class ControlConditionerType, class ControlConditionerType_>
  static ControlConditionerType operator >> (ControlGenerator lhs, TemplatedControlConditioner<ControlConditionerType, ControlConditionerType_> rhs){
    return rhs.input( lhs );
  }
  
}

#endif /* defined(__TonicDemo__ControlConditioner__) */
