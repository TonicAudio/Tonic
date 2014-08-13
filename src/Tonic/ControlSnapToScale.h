//
//  ControlSnapToScale.h
//  Tonic 
//
//  Created by Morgan Packard on 3/24/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_CONTROLSNAPTOSCALE_H
#define TONIC_CONTROLSNAPTOSCALE_H

#include "ControlConditioner.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlSnapToScale_ : public ControlConditioner_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
      vector<float> mScale;
      float snap(float number);
      
    public:

      void setScale(vector<float> scale) { mScale = scale; }
      
    };
    
    inline void ControlSnapToScale_::computeOutput(const SynthesisContext_ & context){
      
      static const int NOTES_PER_OCTAVE = 12;
      
      if( input_.tick(context).triggered ){
        
        float number = input_.tick(context).value;
        
        int octave = number / NOTES_PER_OCTAVE;
        float baseNumber = number - (octave * NOTES_PER_OCTAVE);
        float snappedValue = snap(baseNumber) + (octave * NOTES_PER_OCTAVE);
        if(output_.value != snappedValue ){
          output_.value = snappedValue;
          output_.triggered = true;
        }else{
          output_.triggered = false;
        }
      }else{
        output_.triggered = false;
      }
    }
    
    inline float ControlSnapToScale_::snap(float number){
      float ret = 0;
      
      float leastDistance = -1;
      for(int i = 0; i<mScale.size(); i++){
        float distance = mScale.at(i) - number;
        if(distance < 0){
          distance = -distance;
        }
        if(leastDistance == -1){
          leastDistance = distance;
        }
        if(distance <= leastDistance){
          leastDistance = distance;
          ret = mScale.at(i);
        }
      }
      return ret;
    }
    
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

#endif


