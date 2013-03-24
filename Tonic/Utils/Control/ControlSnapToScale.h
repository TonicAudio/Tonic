//
//  ControlSnapToScale.h
//  Tonic 
//
//  Created by Morgan Packard on 3/24/13.
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

#ifndef __Tonic__ControlSnapToScale__
#define __Tonic__ControlSnapToScale__

#include "ControlConditioner.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlSnapToScale_ : public ControlConditioner_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
      vector<float> mScale;
      float snap(float number);
      
    public:
      ControlSnapToScale_();
      ~ControlSnapToScale_();
      void setScale(vector<float>);
      
    };
    
  }
  
  /*!
    Given a scale, snaps the input value to the nearest scale note, in any octave. 
  */
  
  class ControlSnapToScale : public TemplatedControlConditioner<ControlSnapToScale, Tonic_::ControlSnapToScale_>{
  
  public:

    ControlSnapToScale setScale(vector<float> scale){
      gen()->setScale(scale);
      return *this;
    }

  };
}

#endif /* defined(__Tonic__ControlSnapToScale__) */


