//
//  ControlChangeNotifier.h
//  Tonic 
//
//  Created by Morgan Packard on 5/11/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ControlChangeNotifier__
#define __Tonic__ControlChangeNotifier__

#include "ControlGenerator.h"
#include "ControlConditioner.h"

namespace Tonic {

  class ControlChangeSubscriber{
    public:
    virtual ~ControlChangeSubscriber(){}
    virtual void valueChanged(string, TonicFloat)=0;
  };
  
  namespace Tonic_ {

    class ControlChangeNotifier_ : public ControlConditioner_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
      ControlChangeSubscriber* valueChangedCallback;
      bool outputReadyToBeSentToUI;
      ControlGeneratorOutput outputToSendToUI;
      
    public:
      ControlChangeNotifier_();
      ~ControlChangeNotifier_();
      void setValueChangedCallback(ControlChangeSubscriber* resp){valueChangedCallback = resp;};
      void tickUI();
      string name;
    };
    
  }
  
  class ControlChangeNotifier : public TemplatedControlConditioner<Tonic::ControlChangeNotifier, Tonic_::ControlChangeNotifier_>{
    
  public:
  
    void tickUI();
    void setValueChangedCallback(ControlChangeSubscriber* resp){gen()->setValueChangedCallback(resp);};
    void setName(string name){gen()->name = name;}

  };
}

#endif /* defined(__Tonic__ControlChangeNotifier__) */


