//
//  ControlBooleanLogic.h
//  Tonic 
//
//  Created by Nick Donaldson on 5/31/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ControlBooleanLogic__
#define __Tonic__ControlBooleanLogic__

#include "ControlValue.h"

namespace Tonic {
  
  // --------------------------------------
  //      BASE LOGIC CONTROLGENERATOR
  // --------------------------------------
  
  namespace Tonic_ {
    
    class ControlBooleanOperator_ : public ControlGenerator_ {
      
    protected:
            
      ControlGenerator lhsGen_;
      ControlGenerator rhsGen_;
      
      
    public:
      
      ControlBooleanOperator_();
      
      void setLeft( ControlGenerator gen ) { lhsGen_ = gen; }
      void setRight( ControlGenerator gen ) { rhsGen_ = gen; }
      
    };
    
  }
  
  template <class OperatorType>
  class TemplatedControlBooleanOperator : public TemplatedControlGenerator<OperatorType> {
    
  public:
    
    void setLeft( ControlGenerator gen ){
      this->gen()->setLeft(gen);
    }
    
    void setRight( ControlGenerator gen ){
      this->gen()->setRight(gen);
    }
    
  };
  
  // --------------------------------------
  //              EQUALITY
  // --------------------------------------
  
  // TODO: Implement
  
  // --------------------------------------
  //             INEQUALITY
  // --------------------------------------
  
  // TODO: Implement
  
  // --------------------------------------
  //            GREATER THAN
  // --------------------------------------
  
  namespace Tonic_ {

    class ControlGreaterThan_ : public ControlBooleanOperator_ {
      
    protected:
      
      void computeOutput(const SynthesisContext_ & context);
      
    };
    
  }
  
  class ControlGreaterThan : public TemplatedControlBooleanOperator<Tonic_::ControlGreaterThan_> {};
  
  static ControlGreaterThan operator > (ControlGenerator lhs, ControlGenerator rhs){
    ControlGreaterThan gt;
    gt.setLeft(lhs);
    gt.setRight(rhs);
    return gt;
  }
  
  static ControlGreaterThan operator > (ControlGenerator lhs, float right){
    ControlGreaterThan gt;
    gt.setLeft(lhs);
    gt.setRight(ControlValue(right));
    return gt;
  }
  
  // --------------------------------------
  //          GREATER OR EQUAL
  // --------------------------------------
  
  // TODO: Implement
  
  // --------------------------------------
  //              LESS THAN
  // --------------------------------------
  
  // TODO: Implement
  
  // --------------------------------------
  //           LESS OR EQUAL
  // --------------------------------------
  
  // TODO: Implement
  
}

#endif /* defined(__Tonic__ControlBooleanLogic__) */


