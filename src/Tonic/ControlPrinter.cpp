//
//  ControlPrinter.cpp
//  Tonic
//
//  Created by Morgan Packard on 4/28/13.
//

#include "ControlPrinter.h"

namespace Tonic { namespace Tonic_{
  
  ControlPrinter_::ControlPrinter_()
    :message("%f\n"), hasPrinted(false){
    
  }
  
  void ControlPrinter_::setMessage(string messageArg){
    message = "Tonic::ControlPrinter message:" +  messageArg + "\n";
  }
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
