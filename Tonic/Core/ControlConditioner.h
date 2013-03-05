//
//  ControlConditioner.h
//  TonicDemo
//
//  Created by Morgan Packard on 3/4/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
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
      
    public:
      
      void in( ControlGenerator & input );
      
    protected:
    
      ControlGenerator input_;
      
    };
    
  }
  
  template<class ControlConditionerType, class ControlConditionerType_>
  class TemplatedControlConditioner : public TemplatedControlGenerator<ControlConditionerType_>{
    
  public:
    
    ControlConditionerType & in( ControlGenerator input ){
      // why doesn't this compile without 'this' ?
      this->gen()->in( input );
      return this;
    }
    
  };
  
    // signal flow operator - sets lhs as input to rhs
  template<class ControlConditionerType, class ControlConditionerType_>
  static ControlConditionerType & operator>>(ControlGenerator lhs, TemplatedControlConditioner<ControlConditionerType, ControlConditionerType_> rhs){
    return rhs.input( lhs );
  }
  
}

#endif /* defined(__TonicDemo__ControlConditioner__) */
