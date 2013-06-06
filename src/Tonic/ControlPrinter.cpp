//
//  ControlPrinter.cpp
//  Tonic
//
//  Created by Morgan Packard on 4/28/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "ControlPrinter.h"

namespace Tonic { namespace Tonic_{
  
  ControlPrinter_::ControlPrinter_()
    :message("%f\n"), hasPrinted(false){
    
  }
  
  ControlPrinter_::~ControlPrinter_(){
    
  }
  
  void ControlPrinter_::computeOutput(const SynthesisContext_ & context){
    output_ = input_.tick(context);
    if(!hasPrinted ||  (output_.triggered)){
      printf(message.c_str(), output_.value);
      hasPrinted = true;
    }
  }
  
  void ControlPrinter_::setMessage(string messageArg){
    message = messageArg + "\n";
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
