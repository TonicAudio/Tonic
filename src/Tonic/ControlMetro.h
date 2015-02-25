//
//  ControlMetro.h
//  Tonic 
//
//  Created by Nick Donaldson on 3/10/13.

//
// See LICENSE.txt for license and usage information.
//



#ifndef TONIC_CONTROLMETRO_H
#define TONIC_CONTROLMETRO_H

#include "ControlGenerator.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlMetro_ : public ControlGenerator_{
      
    protected:
      
      double lastClickTime_;
	  bool isRunning_;
      ControlGenerator bpm_;
	  ControlGenerator startTrigger_;
	  ControlGenerator stopTrigger_;

      void computeOutput(const SynthesisContext_ & context);
          
    public:

      ControlMetro_();
      
      void setBPMGen( ControlGenerator bpmGen ){ bpm_ = bpmGen; };
	  void setStartTrigger(ControlGenerator gen){ startTrigger_ = gen; };
	  void setStopTrigger(ControlGenerator gen){ stopTrigger_ = gen; };
      
    };
    
    inline void ControlMetro_::computeOutput(const SynthesisContext_ & context){
      
      if (startTrigger_.tick(context).triggered)
	  {
        lastClickTime_ = 0; // reset
        isRunning_ = true;
      }
      if(stopTrigger_.tick(context).triggered){
        isRunning_ = false;
      }

	  if (isRunning_)
	  {
		  double sPerBeat = 60.0 / max(0.001, bpm_.tick(context).value);
		  double delta = context.elapsedTime - lastClickTime_;
		  if (delta >= 2 * sPerBeat || delta < 0){
			  // account for bpm interval outrunning tick interval or timer wrap-around
			  lastClickTime_ = context.elapsedTime;
			  output_.triggered = true;
		  }
		  else if (delta >= sPerBeat){
			  // account for drift
			  lastClickTime_ += sPerBeat;
			  output_.triggered = true;
		  }
		  else{
			  output_.triggered = false;
		  }

	  }
	  else{
		  output_.triggered = false;
	  }
	  


      output_.value = 1;
      
    }
    
  }
  
  //!Ouputs a "changed" status at a regular BPM interval
  class ControlMetro : public TemplatedControlGenerator<Tonic_::ControlMetro_>{
    
  public:
    
    ControlMetro(float bpm = 120){
      gen()->setBPMGen(ControlValue(bpm));
    }
    
	TONIC_MAKE_CTRL_GEN_SETTERS(ControlMetro, bpm, setBPMGen);
	TONIC_MAKE_CTRL_GEN_SETTERS(ControlMetro, startTrigger, setStartTrigger);
	TONIC_MAKE_CTRL_GEN_SETTERS(ControlMetro, stopTrigger, setStopTrigger);
  };
}

#endif


