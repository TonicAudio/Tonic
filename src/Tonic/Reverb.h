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
#include "MonoToStereoPanner.h"


namespace Tonic {
  
  namespace Tonic_ {
    
    //! Allpass filter for use with reverb
    class ImpulseDiffuserAllpass {
      
    protected:
      
      DelayLine delayForward_;
      DelayLine delayBack_;
      TonicFloat delay_;
      TonicFloat coef_;
      
    public:
      
      ImpulseDiffuserAllpass(TonicFloat delay, TonicFloat coef);
      ImpulseDiffuserAllpass( const ImpulseDiffuserAllpass & other);
      void tickThrough(TonicFrames & frames);
      
    };
    
    inline void ImpulseDiffuserAllpass::tickThrough(Tonic::TonicFrames &frames)
    {
      TonicFloat *dptr = &frames[0];
      TonicFloat y;
      for (int i=0; i<kSynthesisBlockSize; i++){
        
        // feedback stage
        y = *dptr + delayBack_.tickOut(delay_) * coef_;
        delayBack_.tickIn(y);
        delayBack_.advance();
        
        // feed forward stage
        *dptr++ = (1.f+coef_)*delayForward_.tickOut(delay_) - y;
        delayForward_.tickIn(y);
        delayForward_.advance();
      }
    }
    
    //! Moorer-Schroeder style Artificial Reverb effect
    /*!
        - [x] Pre-delay
        - [x] Input filter
        - [x] Early reflection taps
        - [x] Decay time and decay filtering
        - [x] Variable "Room size"
        - [x] Variable stereo width
     */
    
    class Reverb_ : public Effect_
    {
      protected:
      
        // Filters and delay lines
        DelayLine     preDelayLine_;
        DelayLine     reflectDelayLine_;
      
        LPF12           inputLPF_;
        HPF12           inputHPF_;
      
        vector<TonicFloat> reflectTapTimes_;
        vector<TonicFloat> reflectTapScale_;

        // Comb filters
        vector<FilteredFBCombFilter6> combFilters_[2];
        vector<ControlValue>          combFilterDelayTimes_[2];
        vector<ControlValue>          combFilterScaleFactors_[2];
      
        // Allpass filters
        vector<ImpulseDiffuserAllpass> allpassFilters_[2];
      
        // Signal vector workspaces
        TonicFrames   workspaceFrames_[2];
        TonicFrames   preOutputFrames_[2];
      
        // Input generators
        ControlGenerator  preDelayTimeCtrlGen_;
        ControlGenerator  inputFiltBypasCtrlGen_;
        ControlGenerator  roomSizeCtrlGen_;
        ControlGenerator  roomShapeCtrlGen_;
        ControlGenerator  densityCtrlGen_; // affects number of early reflection taps
      
        ControlGenerator decayTimeCtrlGen_;
        ControlGenerator stereoWidthCtrlGen_;
      
        void updateDelayTimes(const SynthesisContext_ & context);
            
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
        void setDecayTimeCtrlGen( ControlGenerator gen ) { decayTimeCtrlGen_ = gen; }
        void setStereoWidthCtrlGen( ControlGenerator gen ) { stereoWidthCtrlGen_ = gen; }
      
        // These are special setters, they will be passed to all the comb filters
        void setDecayLPFCtrlGen( ControlGenerator gen );
        void setDecayHPFCtrlGen( ControlGenerator gen );
      
    };
    
    inline void Reverb_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      updateDelayTimes(context);
      
      // pass thru input filters
      if (inputFiltBypasCtrlGen_.tick(context).value == 0.f){
        
        inputLPF_.tickThrough(dryFrames_, workspaceFrames_[0], context);
        inputHPF_.tickThrough(workspaceFrames_[0], workspaceFrames_[0], context);
        
      }
      else{
        workspaceFrames_[0].copy(dryFrames_);
      }
      
      TonicFloat *wkptr0 = &(workspaceFrames_[0])[0];
      TonicFloat *wkptr1 = &(workspaceFrames_[1])[0];
      
      // pass thru pre-delay, input filters, and sum the early reflections
      
      TonicFloat preDelayTime = preDelayTimeCtrlGen_.tick(context).value;
      
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
      
        // filtered input is in w0
        // predelay output is in w1
        
        // pre-delay
        preDelayLine_.tickIn(*wkptr0);
        *wkptr1 = preDelayLine_.tickOut(preDelayTime);
        preDelayLine_.advance();
        
        // taps - write back to w0
        reflectDelayLine_.tickIn(*wkptr1++);
        *wkptr0 = 0;
        for (unsigned int t=0; t<reflectTapTimes_.size(); t++){
          *wkptr0 += reflectDelayLine_.tickOut(reflectTapTimes_[t]) * reflectTapScale_[t];
        }
        
        reflectDelayLine_.advance();
        wkptr0++;
      }
      
      // Comb filers
      preOutputFrames_[TONIC_LEFT].clear();
      preOutputFrames_[TONIC_RIGHT].clear();
      for (unsigned int i=0; i<combFilters_[TONIC_LEFT].size(); i++){
        combFilters_[TONIC_LEFT][i].tickThrough(workspaceFrames_[0], workspaceFrames_[1], context);
        preOutputFrames_[TONIC_LEFT] += workspaceFrames_[1];
        combFilters_[TONIC_RIGHT][i].tickThrough(workspaceFrames_[0], workspaceFrames_[1], context);
        preOutputFrames_[TONIC_RIGHT] += workspaceFrames_[1];
      }
      
      // Allpass filters
      for (unsigned int i=0; i<allpassFilters_[TONIC_LEFT].size(); i++){
        allpassFilters_[TONIC_LEFT][i].tickThrough(preOutputFrames_[TONIC_LEFT]);
        allpassFilters_[TONIC_RIGHT][i].tickThrough(preOutputFrames_[TONIC_RIGHT]);
      }
      
      // interleave pre-output frames into output frames
      TonicFloat *outptr = &outputFrames_[0];
      TonicFloat *preoutptrL = &preOutputFrames_[TONIC_LEFT][0];
      TonicFloat *preoutptrR = &preOutputFrames_[TONIC_RIGHT][0];
      
      TonicFloat spreadValue = clamp(1.0f - stereoWidthCtrlGen_.tick(context).value, 0.f, 1.f);
      TonicFloat normValue = (1.0f/(1.0f+spreadValue))*0.05;
      
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
        *outptr++ = (*preoutptrL + (spreadValue * (*preoutptrR)))*normValue;
        *outptr++ = (*preoutptrR++ + (spreadValue * (*preoutptrL++)))*normValue;
      }


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
    
      //! Value in seconds of overall decay time
      createControlGeneratorSetters(Reverb, decayTime, setDecayTimeCtrlGen);
    
      //! Value in Hz of cutoff of decay LPF
      createControlGeneratorSetters(Reverb, decayLPFCutoff, setDecayLPFCtrlGen);
    
      //! Value in Hz of cutoff of decay HPF
      createControlGeneratorSetters(Reverb, decayHPFCutoff, setDecayHPFCtrlGen);

      //! Value 0-1 for stereo width
      createControlGeneratorSetters(Reverb, stereoWidth, setStereoWidthCtrlGen);
    
  };
}

#endif /* defined(__Tonic__Reverb__) */

