//
//  ControlTimingUtils.h
//  Tonic 
//
//  Created by Nick Donaldson on 3/10/13.

//
// See LICENSE.txt for license and usage information.
//



#ifndef __Tonic__ControlTimingUtils__
#define __Tonic__ControlTimingUtils__

#include "ControlGenerator.h"
#include "ControlConditioner.h"

namespace Tonic {
  
  namespace Tonic_ {

    //! Control-rate metronome which outputs triggers at a regular bpm
    class ControlMetro_ : public ControlGenerator_{
      
    protected:
      
      double lastClickTime_;
      
      ControlGenerator bpm_;
      
      void computeOutput(const SynthesisContext_ & context);
          
    public:

      ControlMetro_();
      
      void setBPMGen( ControlGenerator bpmGen ){ bpm_ = bpmGen; };
      
    };
    
    inline void ControlMetro_::computeOutput(const SynthesisContext_ & context){
      
      double sPerBeat = 60.0/max(0.001,bpm_.tick(context).value);
      double delta = context.elapsedTime - lastClickTime_;
      if (delta >= 2*sPerBeat || delta < 0){
        // account for bpm interval outrunning tick interval or timer wrap-around
        lastClickTime_ = context.elapsedTime;
        output_.triggered = true;
      }
      else if (delta >= sPerBeat){
        // acocunt for drift
        lastClickTime_ += sPerBeat;
        output_.triggered = true;
      }
      else{
        output_.triggered = false;
      }
      
      output_.value = 1;
      
    }
    
    // ------------------------------------------------------------
    
    //! Produces output triggers at a sub-interval of the incoming triggers
    class ControlMetroDivider_ : public ControlConditioner_ {
      
    protected:
      
      void computeOutput(const SynthesisContext_ & context);
      
      ControlGenerator divisonsGen_;
      ControlGenerator offsetGen_;
      
      unsigned int offset_;
      unsigned int tickCounter_;
      
    public:
      
      ControlMetroDivider_();
      
      void setDivisionsGen( ControlGenerator gen ) { divisonsGen_ = gen; }
      void setOffsetGen( ControlGenerator gen ) { offsetGen_ = gen; }
      
    };
    
    inline   void ControlMetroDivider_::computeOutput(const SynthesisContext_ & context){
      
      offset_ = (unsigned int)(offsetGen_.tick(context).value);
      unsigned int divisions = max(1, divisonsGen_.tick(context).value);
      
      output_.triggered = false;
      
      if (input_.tick(context).triggered)
      {
        unsigned int modcount = (tickCounter_++ + offset_) % divisions;
        if (modcount == 0){
          output_.triggered = true;
        }
        
        if (tickCounter_ >= divisions) tickCounter_ = 0;
      }
      
    }
    
    // ------------------------------------------------------------
  
    class ControlPulse_ : public ControlConditioner_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
      
      typedef enum {
        ControlPulseStateOff = 0,
        ControlPulseStateOn
      } ControlPulseState;
      
      ControlPulseState state_;
      double lastOnTime_;
      
      ControlGenerator pulseLengthGen_;
      
    public:
      
      ControlPulse_();
      void setPulseLengthGen( ControlGenerator gen ){ pulseLengthGen_ = gen; };
      
    };
    
    inline   void ControlPulse_::computeOutput(const SynthesisContext_ & context){
      
      ControlGeneratorOutput tickIn = input_.tick(context);
      ControlGeneratorOutput lengthIn = pulseLengthGen_.tick(context);
      
      output_.triggered = false;
      
      // every time input changes, reset status, start new pulse
      if (tickIn.triggered){
        state_ = ControlPulseStateOn;
        lastOnTime_ = context.elapsedTime;
        output_.triggered = true;
        output_.value = 1.0f;
      }
      else if (state_ == ControlPulseStateOn){
        
        double tDiff = context.elapsedTime - lastOnTime_;
        
        if (tDiff < 0 || tDiff >= max(0,lengthIn.value)){
          state_ = ControlPulseStateOff;
          output_.value = 0.0f;
          output_.triggered = true;
        }
      }
      
    }
    
    // ------------------------------------------------------------

    class ControlDelay_ : public ControlConditioner_{
      
    private:
      
      long readHead_;
      long writeHead_;
      
      long maxDelay_; // # synthesis blocks of delay
      
      std::vector<ControlGeneratorOutput> delayLine_;
      
      ControlGenerator delayTimeCtrlGen_;
      
      void computeOutput(const SynthesisContext_ & context);
      
    public:
      
      ControlDelay_();
      
      void initialize(float maxDelayTime);
      
      void setDelayTimeGen( ControlGenerator gen ){ delayTimeCtrlGen_ = gen; };
      
    };
    
    inline     void ControlDelay_::computeOutput(const SynthesisContext_ & context){
      
      delayLine_[writeHead_] = input_.tick(context);
      
      ControlGeneratorOutput delayTimeOutput = delayTimeCtrlGen_.tick(context);
      if (delayTimeOutput.triggered){
        
        unsigned delayBlocks = max(delayTimeOutput.value * sampleRate() / kSynthesisBlockSize, 1);
        
#ifdef TONIC_DEBUG
        if (delayBlocks >= maxDelay_){
          debug("ControlDelay: delay time greater than maximum delay (defaults to 1 scond). Use constructor to set max delay -- ex. ControlDelay(2.0))");
        }
#endif
        
        readHead_ = writeHead_ - delayBlocks;
        if (readHead_ < 0) readHead_ += maxDelay_;
        
      }
      
      output_ = delayLine_[readHead_];
      
      if (++writeHead_ >= maxDelay_) writeHead_ = 0;
      if (++readHead_ >= maxDelay_) readHead_ = 0;
    }
    
  }
    
  // ------------------------------------------------------------
  
  //! Control-rate metronome which outputs triggers at a regular bpm
  class ControlMetro : public TemplatedControlGenerator<Tonic_::ControlMetro_>{
    
  public:
    
    ControlMetro(float bpm = 120){
      gen()->setBPMGen(ControlValue(bpm));
    }
    
    createControlGeneratorSetters(ControlMetro, bpm, setBPMGen);
  };
  
  // ------------------------------------------------------------
  
  //! Produces output triggers at a sub-interval of the incoming triggers
  class ControlMetroDivider : public TemplatedControlConditioner<ControlMetroDivider, Tonic_::ControlMetroDivider_>
  {
  public:
    
    createControlGeneratorSetters(ControlMetroDivider, divisions, setDivisionsGen);
    createControlGeneratorSetters(ControlMetroDivider, offset, setOffsetGen);
    
  };
  
  // ------------------------------------------------------------
  
  class ControlPulse : public TemplatedControlConditioner<ControlPulse, Tonic_::ControlPulse_>{
    
  public:
    
    ControlPulse(float length = 0.1){
      gen()->setPulseLengthGen(ControlValue(length));
    }
    
    createControlGeneratorSetters(ControlPulse, length, setPulseLengthGen);
    
  };
  
  // ------------------------------------------------------------
  
  class ControlDelay : public TemplatedControlConditioner<ControlDelay, Tonic_::ControlDelay_>{
    
  public:
    
    ControlDelay(float maxDelayTime = 1.0f);
    createControlGeneratorSetters(ControlDelay, delayTime, setDelayTimeGen);
    
  };
}

#endif /* defined(__Tonic__ConrolTimingUtils__) */


