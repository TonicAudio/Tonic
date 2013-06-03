//
//  ControlArithmetic.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/24/13.

//

#include "ControlArithmetic.h"

namespace Tonic { namespace Tonic_{
  
  // -----------------------------------------
  //                 ADDER
  // -----------------------------------------
  
  void ControlAdder_::input(ControlGenerator input){
    inputs.push_back(input);
  }
  
  void ControlAdder_::computeOutput(const SynthesisContext_ &context){
    
    lastOutput_.triggered = false;
    
    for (unsigned int i=0; i<inputs.size(); i++){
      if (inputs[i].tick(context).triggered){
        lastOutput_.triggered = true;
        break;
      }
    }
    
    TonicFloat sum = 0.0f;
    for (unsigned int i=0; i<inputs.size(); i++){
      sum += inputs[i].tick(context).value;
    }
    lastOutput_.value = sum;
  }
  
  // -----------------------------------------
  //               SUBTRACTOR
  // -----------------------------------------

  void ControlSubtractor_::computeOutput(const SynthesisContext_ & context){
    ControlGeneratorOutput leftOut = left.tick(context);
    ControlGeneratorOutput rightOut = right.tick(context);
    if(!leftOut.triggered && !rightOut.triggered){
      lastOutput_.triggered = false;
    }else{
      lastOutput_.triggered = true;
      lastOutput_.value = leftOut.value - rightOut.value;
    }
  }
  
  // -----------------------------------------
  //               MULTIPLIER
  // -----------------------------------------
  
  void ControlMultiplier_::input(ControlGenerator input){
    inputs.push_back(input);
  }
  
  void ControlMultiplier_::computeOutput(const SynthesisContext_ &context){
    
    lastOutput_.triggered = false;
    
    for (unsigned int i=0; i<inputs.size(); i++){
      if (inputs[i].tick(context).triggered){
        lastOutput_.triggered = true;
        break;
      }
    }
    
    TonicFloat product = inputs[0].tick(context).value;
    for (unsigned int i=1; i<inputs.size(); i++){
      product *= inputs[i].tick(context).value;
    }
    lastOutput_.value = product;
  }
  
  // -----------------------------------------
  //                DIVIDER
  // -----------------------------------------
  
  void ControlDivider_::computeOutput(const SynthesisContext_ & context){
    ControlGeneratorOutput leftOut = left.tick(context);
    ControlGeneratorOutput rightOut = right.tick(context);
    
    bool rightIsZero = rightOut.value == 0;
    if(rightIsZero){
      error("ControlGenerator divide by zero encountered. Returning last valid value");
    }
    bool noChange = !leftOut.triggered && !rightOut.triggered;
    if(rightIsZero || noChange){
      lastOutput_.triggered = false;
    }else{
      lastOutput_.triggered = true;
      lastOutput_.value = leftOut.value / rightOut.value;
    }
  }
  

  
} // Namespace Tonic_

} // Namespace Tonic
