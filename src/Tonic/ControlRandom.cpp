//
//  ControlRandom.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 3/14/13.

//

#include "ControlRandom.h"

namespace Tonic{

  namespace Tonic_{
  
    ControlRandom_::ControlRandom_(){
      trigger = ControlValue(0);
    }
  
    void ControlRandom_::computeOutput(const SynthesisContext_ & context){
      ControlGeneratorOutput minOut = min.tick(context);
      ControlGeneratorOutput maxOut = max.tick(context);
    
      bool outInRange =  (output_.value >= minOut.value) && (output_.value <= maxOut.value);
    
      if(!outInRange || trigger.tick(context).triggered){
        output_.triggered = true;
        output_.value = randomFloat(minOut.value, maxOut.value);
      }else{
        output_.triggered = false;
      }
    }
    
  }
}