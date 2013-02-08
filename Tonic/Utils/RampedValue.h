//
//  RampedValue.h
//  Tonic 
//
//  Created by Virek on 2/7/13.
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

namespace Tonic {
  
  namespace Tonic_ {
    
    class RampedValue_ : public Generator_{
      
    protected:
            
      bool finished_;
      unsigned long count_;
      unsigned long defLen_;
      unsigned long len_;
      TonicFloat target_;
      TonicFloat last_;
      TonicFloat inc_;
      
    public:
      RampedValue_();
      ~RampedValue_();
      void tick( TonicFrames& frames);

      // lengths in samples
      inline void setDefaultLength( unsigned long defLen) { defLen_ = defLen; };
      void setValue( TonicFloat value);
      void setTarget( TonicFloat target, unsigned long length );
  
    };
    
    inline void RampedValue_::tick( TonicFrames& frames){
            
      TonicFloat *fdata = &frames[0];
      unsigned int stride = frames.channels();
            
      if (finished_){
        #ifdef USE_APPLE_ACCELERATE
        vDSP_vfill(&last_, fdata, stride, frames.frames());
        #else
        for (unsigned int i=0; i<frames.frames(); i++){
          *fdata = last_;
          fdata += stride;
        }
        #endif
      }
      else{
        
        // figure out if we will finish the ramp in this tick
        unsigned long remainder = count_ > len_ ? 0 : len_ - count_;
        if (remainder < frames.frames()){
          
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
          vDSP_vfill(&target_, fdata + remainder, stride, frames.frames() - remainder);
          #else
          for (unsigned int i=remainder; i<frames.frames(); i++){
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
          TonicFloat segTarget = last_ + frames.frames()*inc_;
          vDSP_vgen(&last_, &segTarget, fdata, stride, frames.frames());
          #else
          TonicFloat val = last_ + inc_;
          for (unsigned int i=0; i<frames.frames(); i++){
            *fdata = val;
            fdata += stride;
            val += inc_;
          }
          #endif
          
          count_ += frames.frames();
          last_ = frames(frames.frames() - 1, 0);
        }
      }
      
      // mono source, so need to fill out channels if necessary
      frames.fillChannels();
      
    }

    inline void RampedValue_::setValue( TonicFloat value){
      finished_ = true;
      last_ = value;
      target_ = value;
      inc_ = 0;
      count_ = 0;
    }

    inline void RampedValue_::setTarget(TonicFloat target, unsigned long length){
      target_ = target;
      count_ = 0;
      len_ = length > 0 ? length : defLen_;
      inc_ = (TonicFloat)(target_ - last_)/len_;
      finished_ = false;
    }
  }
  
  class RampedValue : public TemplatedGenerator<Tonic_::RampedValue_>{
    
  public:
    
    // for initialization
    RampedValue & defLenMs( TonicFloat defLenMs );
    RampedValue & defValue( TonicFloat defValue );
    
    //! Go to target in specified time. If lenMs < 0, uses default length
    void setTarget( TonicFloat target, TonicFloat lenMs = 0);
    
    //! Go to value immediately
    void setValue( TonicFloat value);

  };
}

#endif /* defined(__Tonic__RampedValue__) */


