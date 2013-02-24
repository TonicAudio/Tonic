//
//  RampedValue.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/7/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
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

#ifndef __Tonic__RampedValue__
#define __Tonic__RampedValue__

#include <iostream>
#include "Generator.h"
#include "ControlValue.h"

namespace Tonic {
  
  namespace Tonic_ {
    
    class RampedValue_ : public Generator_{
      
    protected:
      
      bool finished_;
      unsigned long count_;
      unsigned long len_;
      TonicFloat target_;
      TonicFloat last_;
      TonicFloat inc_;
      ControlGenerator targetGen_;
      ControlGenerator lengthGen_;
      ControlGenerator valueGen_;
      
    public:
      RampedValue_();
      ~RampedValue_();
      
      void computeSynthesisBlock( const SynthesisContext_ & context );
      
      void setValueGen( ControlGenerator value );
      void setTargetGen( ControlGenerator target );
      void setLengthMsGen( ControlGenerator lengthMs );
      
      // internal state setters
      void updateValue( TonicFloat value);
      void updateTarget( TonicFloat target, unsigned long lengthSamp );
  
    };
    
    inline void RampedValue_::computeSynthesisBlock( const SynthesisContext_ & context ){
    
      lockMutex();
      
      // Must do this in this order - value will override any change to target
      if (targetGen_.hasChanged(context) || lengthGen_.hasChanged(context)){
        unsigned long lSamp = lengthGen_.getValue(context)*Tonic::sampleRate()/1000.0f;
        updateTarget(targetGen_.getValue(context), lSamp);
      }
      
      if(valueGen_.hasChanged(context)){
        updateValue(valueGen_.getValue(context));
      }

      unlockMutex();
      
      TonicFloat *fdata = &synthesisBlock_[0];
      unsigned int nFrames = synthesisBlock_.frames();
      unsigned int stride = synthesisBlock_.channels();
            
      if (finished_){
        #ifdef USE_APPLE_ACCELERATE
        vDSP_vfill(&last_, fdata, stride, nFrames);
        #else
        for (unsigned int i=0; i<nFrames; i++){
          *fdata = last_;
          fdata += stride;
        }
        #endif
      }
      else{
        
        // figure out if we will finish the ramp in this tick
        unsigned long remainder = count_ > len_ ? 0 : len_ - count_;
        if (remainder < synthesisBlock_.frames()){
          
          // fill part of the ramp
          #ifdef USE_APPLE_ACCELERATE
          vDSP_vgen(&last_, &target_, fdata, stride, remainder);
          #else
          TonicFloat val = last_ + inc_;
          for (unsigned int i=0; i<remainder; i++){
            *fdata = val;
            fdata += stride;
          }
          #endif
          
          // fill the rest of the ramp
          #ifdef USE_APPLE_ACCELERATE
          vDSP_vfill(&target_, fdata + remainder, stride, nFrames - remainder);
          #else
          for (unsigned int i=remainder; i<nFrames; i++){
            *fdata = target_;
            fdata += stride;
          }
          #endif
          
          count_ = len_;
          last_ = target_;
          finished_ = true;
        }
        else{
          
          // fill the whole ramp
          #ifdef USE_APPLE_ACCELERATE
          TonicFloat segTarget = last_ + synthesisBlock_.frames()*inc_;
          vDSP_vgen(&last_, &segTarget, fdata, stride, nFrames);
          #else
          TonicFloat val = last_ + inc_;
          for (unsigned int i=0; i<nFrames; i++){
            *fdata = val;
            fdata += stride;
            val += inc_;
          }
          #endif
          
          count_ += nFrames;
          last_ = synthesisBlock_(nFrames - 1, 0);
        }
      }
      
      // mono source, so need to fill out channels if necessary
      synthesisBlock_.fillChannels();
      
    }
    
#pragma mark - Generator setters
    
    inline void RampedValue_::setValueGen(ControlGenerator value){
      lockMutex();
      valueGen_ = value;
      unlockMutex();
    }
    
    inline void RampedValue_::setTargetGen(ControlGenerator target){
      lockMutex();
      targetGen_ = target;
      unlockMutex();
    }
    
    inline void RampedValue_::setLengthMsGen(ControlGenerator lengthMs){
      lockMutex();
      lengthGen_ = lengthMs;
      unlockMutex();
    }

#pragma mark - Internal State setters
    
    inline void RampedValue_::updateValue( TonicFloat value){
      finished_ = true;
      last_ = value;
      target_ = value;
      inc_ = 0;
      count_ = 0;
      
      // need to set target gen's value here for this to work right
    }

    inline void RampedValue_::updateTarget(TonicFloat target, unsigned long lengthSamp ){
      target_ = target;
      count_ = 0;
      len_ = lengthSamp > 0 ? lengthSamp : 1;
      inc_ = (TonicFloat)(target_ - last_)/len_;
      finished_ = false;
    }
  }
  
  class RampedValue : public TemplatedGenerator<Tonic_::RampedValue_>{
    
  public:
      
    RampedValue(TonicFloat startValue = 0, TonicFloat lenMs = 50);
    
    //! Set target value
    RampedValue & target( TonicFloat target );
    RampedValue & target( ControlGenerator target );
    
    //! Set length before reaching target value, in ms
    RampedValue & lengthMs( TonicFloat lengthMs );
    RampedValue & lengthMs( ControlGenerator lengthMs );
    
    //! Go to value immediately
    RampedValue & value( TonicFloat value);
    RampedValue & value( ControlGenerator value);

  };
}

#endif /* defined(__Tonic__RampedValue__) */


