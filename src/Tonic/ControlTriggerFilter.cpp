//
//  ControlTriggerFilter.cpp
//  Tonic
//
//  Created by Morgan Packard on 5/22/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "ControlTriggerFilter.h"

namespace Tonic { namespace Tonic_{
  
  ControlTriggerFilter_::ControlTriggerFilter_(): step(0){
    loop_ = ControlValue(true);
    finished = false;
  }
  
  ControlTriggerFilter_::~ControlTriggerFilter_(){
    
  }
  
  void ControlTriggerFilter_::computeOutput(const SynthesisContext_ & context){
    ControlGeneratorOutput triggerOut = trigger_.tick(context);
    ControlGeneratorOutput loopOut = loop_.tick(context);
    if(triggerOut.triggered){
      if (finished) {
        output_ = triggerOut;
      }else{
        output_.triggered = sequence_.at(step) ? true : false;
        if(output_.triggered){
          output_.value = triggerOut.value;
        }
        step = (step + 1) % sequence_.size();
        if (step == 0 && !loopOut.value) {
          finished = true;
        }
      }

     
    }else{
      output_.triggered = false;
    }
    
    
  }
  
  
  void  ControlTriggerFilter_::sequence(string seqArg){
    sequence_.clear();
    for(int i = 0; i< seqArg.size(); i++){
      if (seqArg.at(i) == '0' || seqArg.at(i) == '1') {
        sequence_.push_back(seqArg.at(i) == '1');
      }
    }
  }
  
  void ControlTriggerFilter_::trigger(ControlGenerator trigger){
    trigger_ = trigger;
  }
  
  void ControlTriggerFilter_::setLoop(ControlGenerator loop){
    loop_ = loop;
  }
  
  
} // Namespace Tonic_
  
  
} // Namespace Tonic
