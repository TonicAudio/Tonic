//
//  ControlXYSpeed.h
//  Tonic 
//
//  Created by Morgan Packard on 4/8/13.
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

#ifndef __Tonic__ControlXYSpeed__
#define __Tonic__ControlXYSpeed__

#include "ControlGenerator.h"
#include <list>

namespace Tonic {
  
  namespace Tonic_ {

    class ControlXYSpeed_ : public ControlGenerator_{
      
    protected:
    
      list<float> vals;
      ControlGenerator x;
      ControlGenerator y;
      
      float lastYVal;
      float lastXVal;
    
      void computeOutput(const SynthesisContext_ & context);
      
    public:
      ControlXYSpeed_();
      ~ControlXYSpeed_();
      void setX(ControlGenerator x);
      void setY(ControlGenerator y);
      
    };
    
  }
  
  /*!
    Given a point described by an x and y coordinate, calculate the speed that the point is moving. This is designed for 
    mouse/finger speed, but could potentially have other uses.
  */
  
  class ControlXYSpeed : public TemplatedControlGenerator<Tonic_::ControlXYSpeed_>{
    
  public:
  
  createControlGeneratorSetters(ControlXYSpeed, x, setX);
  createControlGeneratorSetters(ControlXYSpeed, y, setY);

  };
}

#endif /* defined(__Tonic__ControlXYSpeed__) */


