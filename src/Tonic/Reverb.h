//
//  Reverb.h
//  Tonic 
//
//  Created by Nick Donaldson on 5/4/13.
//

//
// See LICENSE.txt for license and usage information.
//

#ifndef TONIC_REVERB_H
#define TONIC_REVERB_H

#include "Effect.h"
#include "DelayUtils.h"
#include "CombFilter.h"
#include "Filters.h"
#include "MonoToStereoPanner.h"


namespace Tonic {
  
  namespace Tonic_ {
    
    //! Allpass filter for use with reverb.
    /*!
        This is not a Generator_ subclass and is optimized for the purposes of the Reverb_ class.
    */
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
        - Pre-delay
        - Input filter
        - Early reflection taps
        - Decay time and decay filtering
        - Variable "Room size"
        - Variable stereo width
     
        TODO:
        - Reverb fb comb cutoff setting parameters should be normalized value, not Hz. Not a "true" cutoff.
        - Highpass portion of fb comb filter is too wide. Should be LPF24.
        - More deterministic early reflection time scattering.
     */
    
    class Reverb_ : public WetDryEffect_
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
      TonicFloat normValue = (1.0f/(1.0f+spreadValue))*0.04f; // scale back levels quite a bit
      
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
        *outptr++ = (*preoutptrL + (spreadValue * (*preoutptrR)))*normValue;
        *outptr++ = (*preoutptrR++ + (spreadValue * (*preoutptrL++)))*normValue;
      }


    }
        
  }
  
  class Reverb : public TemplatedWetDryEffect<Reverb, Tonic_::Reverb_>
  {

    public:
        
      //! Initial delay before passing through reverb
      TONIC_MAKE_CTRL_GEN_SETTERS(Reverb, preDelayTime, setPreDelayTimeCtrlGen);
    
      //! Non-zero value will disable input filtering
      TONIC_MAKE_CTRL_GEN_SETTERS(Reverb, bypassInputFilter, setInputFiltBypassCtrlGen);
    
      //! Value in Hz of cutoff for input LPF
      TONIC_MAKE_CTRL_GEN_SETTERS(Reverb, inputLPFCutoff, setInputLPFCutoffCtrlGen);
    
      //! Value in Hz of cutoff for input HPF
      TONIC_MAKE_CTRL_GEN_SETTERS(Reverb, inputHPFCutoff, setInputHPFCutoffCtrlGen);
    
      //! Value 0-1, affects number of early reflections
      TONIC_MAKE_CTRL_GEN_SETTERS(Reverb, density, setDensityCtrlGen);
    
      //! Value 0-1, affects distribution of early reflections.
      /*!
          0 is perfectly square room
          1 is long, narrow room
      */
      TONIC_MAKE_CTRL_GEN_SETTERS(Reverb, roomShape, setRoomShapeCtrlGen);
    
      //! Value 0-1, affects spacing of early reflections
      TONIC_MAKE_CTRL_GEN_SETTERS(Reverb, roomSize, setRoomSizeCtrlGen);
    
      //! Value in seconds of overall decay time
      TONIC_MAKE_CTRL_GEN_SETTERS(Reverb, decayTime, setDecayTimeCtrlGen);
    
      //! Value in Hz of cutoff of decay LPF
      TONIC_MAKE_CTRL_GEN_SETTERS(Reverb, decayLPFCutoff, setDecayLPFCtrlGen);
    
      //! Value in Hz of cutoff of decay HPF
      TONIC_MAKE_CTRL_GEN_SETTERS(Reverb, decayHPFCutoff, setDecayHPFCtrlGen);

      //! Value 0-1 for stereo width
      TONIC_MAKE_CTRL_GEN_SETTERS(Reverb, stereoWidth, setStereoWidthCtrlGen);
    
  };
}

#endif


