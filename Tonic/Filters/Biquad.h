//
//  Biquad.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/20/13.
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

#ifndef __Tonic__Biquad__
#define __Tonic__Biquad__

#include <iostream>
#include "Effect.h"

namespace Tonic {
  
  namespace Tonic_ {
    
    //! Biquad_ is an IIR biquad filter which provides a base object on which to build more advanced filters
    class Biquad_ : public Generator_ {
      
    protected:
            
      TonicFloat coef_[5];
      TonicFrames inputVec_;
      TonicFrames outputVec_;
      
    public:
      Biquad_();
      ~Biquad_();
      void tick( TonicFrames& frames);
      
      //! Set the coefficients for the filtering operation.
      /*
                  b0 + b1*z^-1 + b2*z^-2
          H(z) = ------------------------
                  1 + a1*z^-1 + a2*z^-2
      */
      void setCoefficients( TonicFloat b0, TonicFloat b1, TonicFloat b2, TonicFloat a1, TonicFloat a2 );
      void setCoefficients( TonicFloat *newCoef );
    };
    
    inline void Biquad_::setCoefficients(TonicFloat b0, TonicFloat b1, TonicFloat b2, TonicFloat a1, TonicFloat a2){
      coef_[0] = b0;
      coef_[1] = b1;
      coef_[2] = b2;
      coef_[3] = a1;
      coef_[4] = a2;
    }
    
    inline void Biquad_::setCoefficients(TonicFloat *newCoef){
      memcpy(coef_, newCoef, 5 * sizeof(TonicFloat));
    }
    
    inline void Biquad_::tick( TonicFrames& frames){
      
      // resize vectors to match number of channels (if necessary)
      if (inputVec_.channels() != frames.channels()){
        inputVec_.resize(kSynthesisBlockSize + 2, frames.channels(), 0);
      }
      
      if (outputVec_.channels() != frames.channels()){
        outputVec_.resize(kSynthesisBlockSize + 2, frames.channels(), 0);
      }
      
      // initialize vectors
      memcpy(&inputVec_[0], &inputVec_(kSynthesisBlockSize, 0), 2 * inputVec_.channels() * sizeof(TonicFloat));
      memcpy(&inputVec_(2, 0), &frames[0], frames.size() * sizeof(TonicFloat));
      memcpy(&outputVec_[0], &outputVec_(kSynthesisBlockSize, 0), 2 * outputVec_.channels() * sizeof(TonicFloat));
      
      // perform IIR filter
      
      unsigned int stride = frames.channels();
      
      #ifdef USE_APPLE_ACCELERATE
      for (unsigned int c=0; c<stride; c++){
        vDSP_deq22(&inputVec_(0,c), stride, coef_, &outputVec_(0,c), stride, kSynthesisBlockSize);
      }
      #else
            
      for (unsigned int c=0; c<stride; c++){
        
        TonicFloat* in = &inputVec_(2, c);
        TonicFloat* out = &outputVec_(2, c);
        
        for (unsigned int i=0; i<kSynthesisBlockSize; i++){
          *out = *(in)*coef_[0] + *(in-stride)*coef_[1] + *(in-2*stride)*coef_[2] - *(out-stride)*coef_[3] - *(out-2*stride)*coef_[4];
          in += stride;
          out += stride;
        }
        
      }
      #endif
      
      // copy to frames passed in
      memcpy(&frames[0], &outputVec_(2,0), kSynthesisBlockSize * stride * sizeof(TonicFloat));
    }
    
  }
  
}

#endif /* defined(__Tonic__Biquad__) */


