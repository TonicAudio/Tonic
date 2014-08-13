//
//  ControlDelay.h
//  Tonic 
//
//  Created by Nick Donaldson on 5/5/13.
//
// See LICENSE.txt for license and usage information.
//

#ifndef TONIC_CONTROLDELAY_H
#define TONIC_CONTROLDELAY_H

#include "ControlConditioner.h"

namespace Tonic {
  
  namespace Tonic_ {

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
    
    inline void ControlDelay_::computeOutput(const SynthesisContext_ & context){
      
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
  
  class ControlDelay : public TemplatedControlConditioner<ControlDelay, Tonic_::ControlDelay_>{
    
  public:
    
    ControlDelay(float maxDelayTime = 1.0f);
    TONIC_MAKE_CTRL_GEN_SETTERS(ControlDelay, delayTime, setDelayTimeGen);
    
  };
}

#endif


