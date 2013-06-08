//
//  ControlSequencingUtils.h
//  Tonic 
//
//  Created by Morgan Packard on 4/11/13.
//
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ControlSequencingUtils__
#define __Tonic__ControlSequencingUtils__

#include "ControlGenerator.h"
#include "ControlConditioner.h"

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
    
    // --------------------------------------------------------------
    
    class ControlStepper_ : public ControlGenerator_{
      
    protected:
      
      ControlGenerator start;
      ControlGenerator end;
      ControlGenerator step;
      ControlGenerator trigger;
      ControlGenerator bidirectional;
      int direction;
      bool hasBeenTriggered;
      
      void computeOutput(const SynthesisContext_ & context);
      
    public:
      ControlStepper_();
      void setStart(ControlGenerator arg){start = arg;}
      void setEnd(ControlGenerator arg){end = arg;}
      void setStep(ControlGenerator arg){step = arg;}
      void setTigger(ControlGenerator arg){trigger = arg;}
      void setBidirectional(ControlGenerator arg){bidirectional = arg;}
    };
    
    inline void ControlStepper_::computeOutput(const SynthesisContext_ & context){
      float startVal = start.tick(context).value;
      float endVal = end.tick(context).value;
      float stepVal = step.tick(context).value;
      bool bi = bidirectional.tick(context).value;
      
      output_.triggered = trigger.tick(context).triggered;
      if(hasBeenTriggered){
        if(output_.triggered){
          output_.value += stepVal * direction;
          if (output_.value <= startVal) {
            output_.value = startVal;
            direction = 1;
          }else if(output_.value >= endVal){
            if(bi){
              direction = -1;
            }else{
              output_.value = startVal;
            }
          }
        }
      } else{
        // So first tick will output start value, even if it hasn't been triggered yet
        output_.value = startVal;
        hasBeenTriggered = true;
      }
    }
    
    // --------------------------------------------------------------
    
    class ControlCounter_ : public ControlGenerator_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
      ControlGenerator trigger;
      ControlGenerator end;
      
    public:
      
      ControlCounter_();
      
      void setTrigger(ControlGenerator gen){ trigger = gen;};
      void setEnd(ControlGenerator gen){end = gen;};
      
    };
    
    inline void ControlCounter_::computeOutput(const SynthesisContext_ & context){
      ControlGeneratorOutput tickOut = trigger.tick(context);
      ControlGeneratorOutput endOut = end.tick(context);
      output_.triggered = tickOut.triggered;
      if (tickOut.triggered) {
        output_.value += 1;
        if(output_.value > endOut.value){
          output_.value = 0;
        }
      }
    }
    
    // --------------------------------------------------------------
    
    class ControlSnapToScale_ : public ControlConditioner_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
      vector<float> mScale;
      float snap(float number);
      
    public:
      
      void setScale(vector<float> scale) { mScale = scale; }
      
    };
    
    inline void ControlSnapToScale_::computeOutput(const SynthesisContext_ & context){
      
      static const int NOTES_PER_OCTAVE = 12;
      
      if( input_.tick(context).triggered ){
        
        float number = input_.tick(context).value;
        
        int octave = number / NOTES_PER_OCTAVE;
        float baseNumber = number - (octave * NOTES_PER_OCTAVE);
        float snappedValue = snap(baseNumber) + (octave * NOTES_PER_OCTAVE);
        if(output_.value != snappedValue ){
          output_.value = snappedValue;
          output_.triggered = true;
        }else{
          output_.triggered = false;
        }
      }else{
        output_.triggered = false;
      }
    }
    
    inline float ControlSnapToScale_::snap(float number){
      float ret = 0;
      
      float leastDistance = -1;
      for(int i = 0; i<mScale.size(); i++){
        float distance = mScale.at(i) - number;
        if(distance < 0){
          distance = -distance;
        }
        if(leastDistance == -1){
          leastDistance = distance;
        }
        if(distance <= leastDistance){
          leastDistance = distance;
          ret = mScale.at(i);
        }
      }
      return ret;
    }
    
  }
  
  // --------------------------------------------------------------
  
  //! ControlSwitcher allows you to switch between an unlimited number of inputs (added via addInput). The "active" input is controlled by the value of inputIndex.
  class ControlSwitcher : public TemplatedControlGenerator<Tonic_::ControlSwitcher_>{
    
  public:
    
    ControlSwitcher & setFloatInputs( vector<float> inputs );
    
    createControlGeneratorSetters(ControlSwitcher, addInput, addInput);
    createControlGeneratorSetters(ControlSwitcher, inputIndex, setInputIndex);

  };
  
  // --------------------------------------------------------------

  //! On trigger, outputs a value beginning at start, and incrementing by step toward end, then back again.
  /*!
    Currently it steps up and down. In the future an option may be added to snap back to the beginning.
   */
  class ControlStepper : public TemplatedControlGenerator<Tonic_::ControlStepper_>{
    
  public:
    
    createControlGeneratorSetters(ControlStepper, start, setStart);
    createControlGeneratorSetters(ControlStepper, end, setEnd);
    createControlGeneratorSetters(ControlStepper, step, setStep);
    createControlGeneratorSetters(ControlStepper, trigger, setTigger);
    createControlGeneratorSetters(ControlStepper, bidirectional, setBidirectional);
    
  };
  
  // --------------------------------------------------------------
  
  //! Counts upwards on each trigger until reaching end, then returns to zero
  class ControlCounter : public TemplatedControlGenerator<Tonic_::ControlCounter_>{
    
  public:
    
    createControlGeneratorSetters(ControlCounter, trigger, setTrigger);
    createControlGeneratorSetters(ControlCounter, end, setEnd);
    
  };
  
  //! Given a scale, snaps the input value to the nearest scale note, in any octave.  
  class ControlSnapToScale : public TemplatedControlConditioner<ControlSnapToScale, Tonic_::ControlSnapToScale_>{
    
  public:
    
    ControlSnapToScale setScale(vector<float> scale){
      gen()->setScale(scale);
      return *this;
    }
    
  };
  
}

#endif /* defined(__Tonic__ControlSequencingUtils__) */


