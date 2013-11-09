//
//  FilterUtils.h
//  TonicDemo
//
//  Created by Nick Donaldson on 2/20/13.

//
// See LICENSE.txt for license and usage information.
//
#ifndef TonicDemo_FilterUtils_h
#define TonicDemo_FilterUtils_h

#include "TonicFrames.h"

namespace Tonic {
  
  //! Calculate coefficient for a pole with given time constant to reach -60dB delta in t60s seconds
  inline static TonicFloat t60ToOnePoleCoef( TonicFloat t60s ){
    float coef = expf(-1.0f/((t60s/6.91f) * sampleRate()));
    return (coef == coef) ? coef : 0.f; // catch NaN
  }
  
  //! Calculate coefficient for a pole with a given desired cutoff in hz
  inline static TonicFloat cutoffToOnePoleCoef( TonicFloat cutoffHz ){
    return clamp(expf(-TWO_PI*cutoffHz/sampleRate()), 0.f, 1.f);
  }
  
  //! Tick one sample through one-pole lowpass filter
  inline void onePoleLPFTick( TonicFloat input, TonicFloat & output, TonicFloat coef){
    output = ((1.0f-coef) * input) + (coef * output);
  }
  
  //! Tick one sample through one-pole highpass filter
  inline void onePoleHPFTick( TonicFloat input, TonicFloat & output, TonicFloat coef){
    output = ((1.0-coef) * input) - (coef * output);
  }

  //! Compute coefficients from analog prototype using bilinear transform
  /*!
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
  
#pragma mark - Biquad Class
  
  //! Biquad_ is an IIR biquad filter which provides a base object on which to build more advanced filters
  class Biquad {
    
  protected:
    
    TonicFloat coef_[5];
    TonicFrames inputVec_;
    TonicFrames outputVec_;
    
  public:
    
    Biquad();
    
    void setIsStereo(bool stereo){
      // resize vectors to match number of channels
      inputVec_.resize(kSynthesisBlockSize + 4, stereo ? 2 : 1, 0);
      outputVec_.resize(kSynthesisBlockSize + 4, stereo ? 2 : 1, 0);
    }
    
    //! Set the coefficients for the filtering operation.
    /*
             b0 + b1*z^-1 + b2*z^-2
     H(z) = ------------------------
             1 + a1*z^-1 + a2*z^-2
     */
    void setCoefficients( TonicFloat b0, TonicFloat b1, TonicFloat b2, TonicFloat a1, TonicFloat a2 );
    void setCoefficients( TonicFloat *newCoef );
    
    void filter( TonicFrames &inFrames, TonicFrames &outFrames );
  };
  
  inline void Biquad::setCoefficients(TonicFloat b0, TonicFloat b1, TonicFloat b2, TonicFloat a1, TonicFloat a2){
    coef_[0] = b0;
    coef_[1] = b1;
    coef_[2] = b2;
    coef_[3] = a1;
    coef_[4] = a2;
  }
  
  inline void Biquad::setCoefficients(TonicFloat *newCoef){
    memcpy(coef_, newCoef, 5 * sizeof(TonicFloat));
  }
  
  inline void Biquad::filter( TonicFrames &inFrames, TonicFrames &outFrames ){
    
    // initialize vectors
    memcpy(&inputVec_[0], &inputVec_(kSynthesisBlockSize, 0), 2 * inputVec_.channels() * sizeof(TonicFloat));
    memcpy(&inputVec_(2, 0), &inFrames[0], inFrames.size() * sizeof(TonicFloat));
    memcpy(&outputVec_[0], &outputVec_(kSynthesisBlockSize, 0), 2 * outputVec_.channels() * sizeof(TonicFloat));
    
    // perform IIR filter
    
    unsigned int stride = inFrames.channels();
    
#ifdef USE_APPLE_ACCELERATE
    for (unsigned int c=0; c<stride; c++){
      vDSP_deq22(&inputVec_[0] + c, stride, coef_, &outputVec_[0] + c, stride, kSynthesisBlockSize);
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

#ifdef TONIC_DEBUG
    if(outFrames(0,0) != outFrames(0,0)){
      Tonic::error("Biquad::filter NaN detected.", false);
    }
#endif
    
    // copy to synthesis block
    memcpy(&outFrames[0], &outputVec_(2,0), kSynthesisBlockSize * stride * sizeof(TonicFloat));
  }
  
  
};

#endif
