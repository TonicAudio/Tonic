//
//  ControlSwitcher.h
//  Tonic 
//
//  Created by Morgan Packard on 4/11/13.
//
//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_CONTROLSWITCHER_H
#define TONIC_CONTROLSWITCHER_H

#include "ControlGenerator.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlSwitcher_ : public ControlGenerator_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
     
      vector<ControlGenerator>          inputs_;
      ControlGenerator                  inputIndex_;
      ControlGenerator                  doesWrap_;
      ControlGenerator                  addAfterWrap_;
      std::map<int, ControlGenerator>   triggers_;
      int                               lastInputIndex_;
      int                               currentInputIndex_;
      int                               lastIndexOutputValue;
      
    public:
      
      ControlSwitcher_();
      void addInput(ControlGenerator input);
      void setInputs( vector<ControlGenerator> inputs );
      void setInputIndex(ControlGenerator inputIndexArg);
      void setDoesWrap(ControlGenerator doesWrap);
      void setAddAfterWrap(ControlGenerator addAfterWrap);
      void  setTriggerForIndex(ControlGenerator trigger, int index);
      
    };
    
    
    
  }
  
  /*!
      ControlSwitcher allows you to switch between an unlimited number of inputs (added via addInput). The "active" input is controlled by the value of inputIndex and any iputs passed via triggerForIndex. If the input index changes, the object will send a trigger (formerly known as "hasChanged") message. Currently, the inputIndex must send a trigger message every time it changes, or the change won't be registered.
  */
  
  class ControlSwitcher : public TemplatedControlGenerator<Tonic_::ControlSwitcher_>{
    
  public:
    
    ControlSwitcher & setFloatInputs( vector<float> inputs );
    
    TONIC_MAKE_CTRL_GEN_SETTERS(ControlSwitcher, addInput, addInput);
    TONIC_MAKE_CTRL_GEN_SETTERS(ControlSwitcher, inputIndex, setInputIndex);
    TONIC_MAKE_CTRL_GEN_SETTERS(ControlSwitcher, doesWrap, setDoesWrap);
    
    //! Useful if you want to use this to map to scale degrees. For midi notes, you can use 12 to jump to the next octave when it wraps.
    // who knows. Maybe useful for something else!
    TONIC_MAKE_CTRL_GEN_SETTERS(ControlSwitcher, addAfterWrap, setAddAfterWrap);
    
    ControlSwitcher & triggerForIndex(ControlGenerator trigger, int index);

  };
}

#endif


