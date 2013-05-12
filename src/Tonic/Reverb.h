//
//  Reverb.h
//  Tonic 
//
//  Created by Nick Donaldson on 5/4/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

/*+++++++++++++++++++++ License ++++++++++++++++++++

Use this code for whatever you want. There are NO 
RESTRICTIONS WHATSOVER. Modify it, repackage it, 
sell it, get rich from it, whatever. Go crazy. If 
you want to make mehappy, contribute to this 
project, but feel free to just use the code as a 
starting point for whatever you like.

Note that Tonic is heavily indebted to STK
https://ccrma.stanford.edu/software/stk/

++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef __Tonic__Reverb__
#define __Tonic__Reverb__

#include "Effect.h"
#include "DelayUtils.h"
#include "CombFilter.h"
#include "Filters.h"

// Number of FF comb filters
#define  TONIC_REVERB_N_COMBS 8

namespace Tonic {
  
  namespace Tonic_ {
    
    //! Moorer-Schroeder style Artificial Reverb effect
    /*!
        - [ ] Pre-delay
        - [ ] Input filter
        - [ ] Early reflection taps
        - [ ] Decay time and decay filtering
        - [ ] Variable "Room size"
        - [ ] Variable stereo width
     */
    
    class Reverb_ : public Effect_
    {
      protected:
      
        // Filters and delay lines
        DelayLine     preDelayLine_;
        DelayLine     reflectDelayLine_;
      
        LPF6          inputLPF_;
        HPF6          inputHPF_;
      
        vector<float> reflectTapTimes_;
        vector<float> reflectTapScale_;

      
        // Signal vector workspaces
        TonicFrames   workSpace_[2];
      
        // Input generators
        ControlGenerator  preDelayTimeCtrlGen_;
        ControlGenerator  inputFiltBypasCtrlGen_;
        ControlGenerator  roomSizeCtrlGen_;
        ControlGenerator  roomShapeCtrlGen_;
        ControlGenerator  densityCtrlGen_; // affects number of early reflection taps
      
        void updateTapTimes(const SynthesisContext_ & context);
      
        void computeSynthesisBlock( const SynthesisContext_ &context );

      public:
      
        Reverb_();
      
        void setPreDelayTimeCtrlGen( ControlGenerator gen ) { preDelayTimeCtrlGen_ = gen; }
        void setInputFiltBypassCtrlGen( ControlGenerator gen ) { inputFiltBypasCtrlGen_ = gen; }
        void setInputLPFCutoffCtrlGen( ControlGenerator gen ) { inputLPF_.cutoff(gen); }
        void setInputHPFCutoffCtrlGen( ControlGenerator gen ) { inputHPF_.cutoff(gen); }
        void setRoomSizeCtrlGen( ControlGenerator gen ) { roomSizeCtrlGen_ = gen; }
        void setRoomShapeCtrlGen( ControlGenerator gen ) { roomShapeCtrlGen_ = gen; }
        void setDensityCtrlGen( ControlGenerator gen ) { densityCtrlGen_ = gen; }
    };
    
    inline void Reverb_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      updateTapTimes(context);
      
      // pass thru input filters
      if (inputFiltBypasCtrlGen_.tick(context).value != 0.f){
        
        inputLPF_.tickThrough(dryFrames_, context);
        inputHPF_.tickThrough(dryFrames_, context);
        
      }

      outputFrames_.copy(dryFrames_);
      
      TonicFloat *inptr = &dryFrames_[0];
      TonicFloat *wkptr0 = &(workSpace_[0])[0];
      TonicFloat *wkptr1 = &(workSpace_[1])[0];
      TonicFloat *outptr = &outputFrames_[0];
      
      // pass thru pre-delay, input filters, and sum the early reflections
      TonicFloat preDelayTime = preDelayTimeCtrlGen_.tick(context).value;
      
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
                
        // pre-delay
        preDelayLine_.tickIn(*inptr++);
        *wkptr0 = preDelayLine_.tickOut(preDelayTime);
        preDelayLine_.advance();
        
        // taps
        reflectDelayLine_.tickIn(*wkptr0);
        for (unsigned int t=0; t<reflectTapTimes_.size(); t++){
          *wkptr0 += reflectDelayLine_.tickOut(reflectTapTimes_[t]) * reflectTapScale_[t];
        }
        
        reflectDelayLine_.advance();
        wkptr0++;
        
      }
      
      // TODO: combs
      
      // TODO: allpass
      
      outputFrames_.copy(workSpace_[0]);

    }
        
  }
  
  class Reverb : public TemplatedEffect<Reverb, Tonic_::Reverb_>
  {

    public:
        
      //! Initial delay before passing through reverb
      createControlGeneratorSetters(Reverb, preDelayTime, setPreDelayTimeCtrlGen);
    
      //! Non-zero value will disable input filtering
      createControlGeneratorSetters(Reverb, bypassInputFilter, setInputFiltBypassCtrlGen);
    
      //! Value in Hz of cutoff for input LPF
      createControlGeneratorSetters(Reverb, inputLPFCutoff, setInputLPFCutoffCtrlGen);
    
      //! Value in Hz of cutoff for input HPF
      createControlGeneratorSetters(Reverb, inputHPFCutoff, setInputHPFCutoffCtrlGen);
    
      //! Value 0-1, affects number of early reflections
      createControlGeneratorSetters(Reverb, density, setDensityCtrlGen);
    
      //! Value 0-1, affects distribution of early reflections.
      /*!
          0 is perfectly square room
          1 is long, narrow room
      */
      createControlGeneratorSetters(Reverb, roomShape, setRoomShapeCtrlGen);
    
      //! Value 0-1, affects spacing of early reflections
      createControlGeneratorSetters(Reverb, roomSize, setRoomSizeCtrlGen);
    
  };
}

#endif /* defined(__Tonic__Reverb__) */


