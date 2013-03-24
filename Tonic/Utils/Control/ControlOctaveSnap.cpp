//
//  ControlOctaveSnap.cpp
//  Tonic
//
//  Created by Morgan Packard on 3/24/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "ControlOctaveSnap.h"

namespace Tonic { namespace Tonic_{
  
  ControlOctaveSnap_::ControlOctaveSnap_(){
    
  }
  
  ControlOctaveSnap_::~ControlOctaveSnap_(){
    
  }
  
  
  float ControlOctaveSnap_::snap(float number){
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
  
  void ControlOctaveSnap_::computeOutput(const SynthesisContext_ & context){
    static const int NOTES_PER_OCTAVE = 12;
    
    if( input_.tick(context).status ==  ControlGeneratorStatusHasChanged){
      float number = input_.tick(context).value;
      
      int octave = number / NOTES_PER_OCTAVE;
      float baseNumber = number - (octave * NOTES_PER_OCTAVE);
      //float ret = snap(baseNumber, targets) + (octave * NOTES_PER_OCTAVE);
      lastOutput_.value = snap(baseNumber) + (octave * NOTES_PER_OCTAVE);
    }
    lastOutput_.status = input_.tick(context).status;
  }
  
  
  void ControlOctaveSnap_::setScale(vector<float> scale){
    mScale = scale;
  }
  
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
