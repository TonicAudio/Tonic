//
//  ControlSnapToScale.cpp
//  Tonic
//
//  Created by Morgan Packard on 3/24/13.

//

#include "ControlSnapToScale.h"

namespace Tonic { namespace Tonic_{
  
  ControlSnapToScale_::ControlSnapToScale_(){
    
  }
  
  ControlSnapToScale_::~ControlSnapToScale_(){
    
  }
  
  
  float ControlSnapToScale_::snap(float number){
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
  
  void ControlSnapToScale_::computeOutput(const SynthesisContext_ & context){
    static const int NOTES_PER_OCTAVE = 12;
    
    if( input_.tick(context).triggered ){
      
      float number = input_.tick(context).value;
      
      int octave = number / NOTES_PER_OCTAVE;
      float baseNumber = number - (octave * NOTES_PER_OCTAVE);
      float snappedValue = snap(baseNumber) + (octave * NOTES_PER_OCTAVE);
      if(lastOutput_.value != snappedValue ){
        lastOutput_.value = snappedValue;
        lastOutput_.triggered = true;
      }else{
        lastOutput_.triggered = false;
      }
    }else{
      lastOutput_.triggered = false;
    }
  }
  
  
  void ControlSnapToScale_::setScale(vector<float> scale){
    mScale = scale;
  }
  
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
