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
      ControlGenerator          doesWrap_;
      ControlGenerator          addAfterWrap_;
      
      int                       lastInputIndex_;
      
    public:
      
      ControlSwitcher_();
      void addInput(ControlGenerator input);
      void setInputs( vector<ControlGenerator> inputs );
      void setInputIndex(ControlGenerator inputIndexArg);
      void setDoesWrap(ControlGenerator doesWrap);
      void setAddAfterWrap(ControlGenerator addAfterWrap);
      
    };
    
    inline void ControlSwitcher_::computeOutput(const SynthesisContext_ & context){
    
      if(inputs_.size() > 0){
            
        for(vector<ControlGenerator>::iterator it = inputs_.begin(); it != inputs_.end(); it++){
          it->tick(context);
        }
        
        ControlGeneratorOutput indexOutput = inputIndex_.tick(context);
        ControlGeneratorOutput doesWrapOut = doesWrap_.tick(context);
        ControlGeneratorOutput addAfterWrapOut = addAfterWrap_.tick(context);
        int index = indexOutput.value;
        
        // always send has changed message when input index changes
        if (indexOutput.triggered && index != lastInputIndex_ && inputs_.size() > 0) {
          lastInputIndex_ = index;
          int cleanedInput = doesWrapOut.value ? index % inputs_.size() : clamp(index, 0, inputs_.size() -1 );
          ControlGeneratorOutput output = inputs_.at(cleanedInput).tick(context);
          output_.triggered = true;
          output_.value = output.value;
          
          // Do the add after wrap thing. Mostly useful for going to scale degrees in higher octaves
          int numTimes = index / inputs_.size();
          output_.value += numTimes * addAfterWrapOut.value;
          
        }else{
          output_.triggered = false;
        }
        
      }
    }
    
  }
  
  /*!
      ControlSwitcher allows you to switch between an unlimited number of inputs (added via addInput). The "active" input is controlled by the value of inputIndex.
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

  };
}

#endif /* defined(__Tonic__ControlSwitcher__) */


