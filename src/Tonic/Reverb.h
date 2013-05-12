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
      
        // Signal vector workspaces
        TonicFrames   workSpace_[2];
      
        // Input generators
        ControlGenerator  preDelayTimeCtrlGen_;
        ControlGenerator  inputFiltBypasCtrlGen_;
        ControlGenerator  roomSizeCtrlGen_;
        //ControlGenerator  densityCtrlGen_; // affects number of early reflection taps
      
      
        void computeSynthesisBlock( const SynthesisContext_ &context );

      public:
      
        Reverb_();
    
        // Overridden so output channel layout follows input channel layout
        void setInput( Generator input );
      
        void setPreDelayTimeCtrlGen( ControlGenerator gen ) { preDelayTimeCtrlGen_ = gen; }
        void setInputFiltBypassCtrlGen( ControlGenerator gen ) { inputFiltBypasCtrlGen_ = gen; }
        void setRoomSizeCtrlGen( ControlGenerator gen ) { roomSizeCtrlGen_ = roomSizeCtrlGen_; }
        void setInputLPFCutoffCtrlGen( ControlGenerator gen ) { inputLPF_.cutoff(gen); }
        void setInputHPFCutoffCtrlGen( ControlGenerator gen ) { inputHPF_.cutoff(gen); }
        
    };
    
    inline void Reverb_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      // TODO: update early reflection tap times here

      // Send dry input to output, apply mix level
//      dryLevelGen_.tick(workSpace_[0], context);
//      outputFrames_.copy(dryFrames_);
//      outputFrames_ *= workSpace_[0];
      
      // pass thru input filters
      if (inputFiltBypasCtrlGen_.tick(context).value == 0.f){
        
        inputLPF_.tickThrough(dryFrames_, context);
        inputHPF_.tickThrough(dryFrames_, context);
        
      }
      
      TonicFloat *inptr = &dryFrames_[0];
      TonicFloat *wkptr0 = &(workSpace_[0])[0];
      TonicFloat *wkptr1 = &(workSpace_[1])[0];
      TonicFloat *outptr = &outputFrames_[0];
      
      // pass thru pre-delay, input filters, and sum the early reflections
      TonicFloat preDelayTime = preDelayTimeCtrlGen_.tick(context).value;
      
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
        
        // dry input to output
        
        // pre-delay
        preDelayLine_.tickIn(*inptr);
        *wkptr0 = preDelayLine_.tickOut(preDelayTime);
        preDelayLine_.advance();
        
        // taps
        reflectDelayLine_.tickIn(*wkptr0);
        for (unsigned int t=0; t<reflectTapTimes_.size(); t++){
          *wkptr0 += reflectDelayLine_.tickOut(reflectTapTimes_[t]);
        }
        
        reflectDelayLine_.advance();
        wkptr0++;
        
      }
      
      // TODO: combs
      
      // TODO: allpass
      
      // Final output is in workSpace_[0]
//      wetLevelGen_.tick(workSpace_[1], context);
//      workSpace_[0] *= workSpace_[1];
//      outputFrames_.copy(workSpace_[0]);
      
    }
        
  }
  
  class Reverb : public TemplatedEffect<Reverb, Tonic_::Reverb_>
  {

    public:
    
      Reverb();
    
      createControlGeneratorSetters(Reverb, preDelayTime, setPreDelayTimeCtrlGen);
      createControlGeneratorSetters(Reverb, bypassInputFilter, setInputFiltBypassCtrlGen);
      createControlGeneratorSetters(Reverb, roomSize, setRoomSizeCtrlGen);
    
  };
}

#endif /* defined(__Tonic__Reverb__) */


