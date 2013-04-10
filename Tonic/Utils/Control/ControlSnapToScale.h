//
//  ControlSnapToScale.h
//  Tonic 
//
//  Created by Morgan Packard on 3/24/13.

//
// See LICENSE.txt for license and usage information.
//


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


