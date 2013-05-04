//
//  ControlXYSpeed.cpp
//  Tonic
//
//  Created by Morgan Packard on 4/8/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "ControlXYSpeed.h"

namespace Tonic { namespace Tonic_{
  
  ControlXYSpeed_::ControlXYSpeed_(): lastYVal(0), lastXVal(0){
    
  }
  
  ControlXYSpeed_::~ControlXYSpeed_(){
    
  }
  
  void ControlXYSpeed_::computeOutput(const SynthesisContext_ & context){
    const int numToAverage = 1;
    ControlGeneratorOutput xOut = x.tick(context);
    ControlGeneratorOutput yOut = y.tick(context);
    if(xOut.status == ControlGeneratorStatusHasChanged || yOut.status == ControlGeneratorStatusHasChanged){
      lastOutput_.status = ControlGeneratorStatusHasChanged;
      float dX = xOut.value - lastXVal;
      float dY = yOut.value  - lastYVal;
      float speed = sqrt(dX * dX + dY * dY);
      vals.push_back(speed);
      if(vals.size() > numToAverage){
        vals.pop_front();
      }
      float total = 0;
      for(list<float>::iterator it = vals.begin(); it != vals.end(); it++){
        total += *it;
      }
      lastOutput_.value = total / vals.size();
      lastXVal = xOut.value;
      lastYVal = yOut.value;
    }else{
      lastOutput_.status = ControlGeneratorStatusHasNotChanged;
    }
    
  }
  
  
  void  ControlXYSpeed_::setX(ControlGenerator xArg){
    x = xArg;
  }
  
  void  ControlXYSpeed_::setY(ControlGenerator yArg){
    y = yArg;
  }
  
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
