//
//  ControlSwitcher.h
//  Tonic 
//
//  Created by Morgan Packard on 4/11/13.
//
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ControlSwitcher__
#define __Tonic__ControlSwitcher__

#include "ControlGenerator.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlSwitcher_ : public ControlGenerator_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
     
      vector<ControlGenerator>  inputs_;
      ControlGenerator          inputIndex_;
      
      int                       lastInputIndex_;
      
    public:
      
      ControlSwitcher_();
      void addInput(ControlGenerator input);
      void setInputs( vector<ControlGenerator> inputs );
      void setInputIndex(ControlGenerator inputIndexArg);
      
    };
    
    inline void ControlSwitcher_::computeOutput(const SynthesisContext_ & context){
      
      for(vector<ControlGenerator>::iterator it = inputs_.begin(); it != inputs_.end(); it++){
        it->tick(context);
      }
      
      ControlGeneratorOutput indexOutput = inputIndex_.tick(context);
      int index = indexOutput.value;
      
      // always send has changed message when input index changes
      if (indexOutput.triggered && index != lastInputIndex_) {
        lastInputIndex_ = index;
        ControlGeneratorOutput output = inputs_.at(clamp(index, 0, inputs_.size() -1 )).tick(context);
        output_.triggered = true;
        output_.value = output.value;
      }else{
        output_.triggered = false;
      }
    }
    
  }
  
  /*!
      ControlSwitcher allows you to switch between an unlimited number of inputs (added via addInput). The "active" input is controlled by the value of inputIndex.
  */
  
  class ControlSwitcher : public TemplatedControlGenerator<Tonic_::ControlSwitcher_>{
    
  public:
    
    ControlSwitcher & setFloatInputs( vector<float> inputs );
    
    createControlGeneratorSetters(ControlSwitcher, addInput, addInput);
    createControlGeneratorSetters(ControlSwitcher, inputIndex, setInputIndex);

  };
}

#endif /* defined(__Tonic__ControlSwitcher__) */


