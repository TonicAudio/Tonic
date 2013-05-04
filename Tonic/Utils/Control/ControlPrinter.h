//
//  ControlPrinter.h
//  Tonic 
//
//  Created by Morgan Packard on 4/28/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ControlPrinter__
#define __Tonic__ControlPrinter__

#include "ControlConditioner.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlPrinter_ : public ControlConditioner_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
      string message;
      bool hasPrinted;
      
    public:
      ControlPrinter_();
      ~ControlPrinter_();
      void setMessage(string message);
      
    };
    
  }
  
  /*
    A controlconditioner which can be used to print the value of its input. You can set an optional format string to customize
    the message using printf-style formatting, with exactly one %f placeholder. For example: "Current bpm is %f.";
  */
  
  class ControlPrinter : public TemplatedControlConditioner<Tonic::ControlPrinter, Tonic_::ControlPrinter_>{
    
  public:
    ControlPrinter message(string message){
      gen()->setMessage(message);
      return *this;
    }

  };
}

#endif /* defined(__Tonic__ControlPrinter__) */


