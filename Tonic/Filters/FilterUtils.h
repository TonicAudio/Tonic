//
//  FilterUtils.h
//  TonicDemo
//
//  Created by Nick Donaldson on 2/20/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_FilterUtils_h
#define TonicDemo_FilterUtils_h

#include "Tonic.h"

namespace Tonic {
  
  //! Calculate coefficient for a pole with given time constant in milliseconds (t60ms)
  inline static TonicFloat msToTc( TonicFloat t60ms ){
    return expf(-1000.0f/(t60ms * Tonic::sampleRate()));
  }

  //! Compute coefficients from analog prototype using bilinear transform
  /*
      Analog transfer function (laplace domain) should look like:
 
              b2 s^2 + b1 s + b0
      H(s) = --------------------
               s^2 + a1 s + a0
 
      And be normalized for a cutoff of 1 rad/s.
   
      fc is the desired frequency cutoff in Hz.
   
      coef_out is a pointer to a TonicFloat array of length 5. No bounds checking is performed.
 
  */
  inline static void bltCoef( TonicFloat b2, TonicFloat b1, TonicFloat b0, TonicFloat a1, TonicFloat a0, TonicFloat fc, TonicFloat *coef_out)
  {
      TonicFloat sf = 1.0f/tanf(PI*fc/Tonic::sampleRate());
      TonicFloat sfsq = sf*sf;
      TonicFloat norm = a0 + a1*sf + sfsq;
      coef_out[0] = (b0 + b1*sf + b2*sfsq)/norm;
      coef_out[1] = 2.0f * (b0 - b2*sfsq)/norm;
      coef_out[2] = (b0 - b1*sf + b2*sfsq)/norm;
      coef_out[3] = 2.0f * (a0 - sfsq)/norm;
      coef_out[4] = (a0 - a1*sf + sfsq)/norm;
  }
    
    
};

#endif
