//
//  ControlChangeNotifier.cpp
//  Tonic
//
//  Created by Morgan Packard on 5/11/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "ControlChangeNotifier.h"

namespace Tonic { namespace Tonic_{
  
  ControlChangeNotifier_::ControlChangeNotifier_() : outputReadyToBeSentToUI(false), valueChangedCallback(NULL){
    
  }
  
  ControlChangeNotifier_::~ControlChangeNotifier_(){
    
  }
  
  void ControlChangeNotifier_::computeOutput(const SynthesisContext_ & context){
    lastOutput_ = input_.tick(context);
    if(lastOutput_.status == ControlGeneratorStatusHasChanged){
      outputReadyToBeSentToUI = false; // maybe silly, but a tiny bit of atomicness/thread safety here
      outputToSendToUI = lastOutput_;
      outputReadyToBeSentToUI = true;
    }
  }
  
  
  void  ControlChangeNotifier_::tickUI(){
    if(outputReadyToBeSentToUI && valueChangedCallback){
      valueChangedCallback->valueChanged(name, outputToSendToUI.value);
      outputReadyToBeSentToUI = false;
    }
  }
  
} // Namespace Tonic_
  
  
  void ControlChangeNotifier::tickUI(){
    gen()->tickUI();
  }

} // Namespace Tonic
