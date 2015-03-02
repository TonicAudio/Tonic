//
//  DSPUtils.h
//  Tonic
//
//  Created by Nick D on 7/21/13.
//
//
//  Largely adapted from:
//  http://www.experimentalscene.com/articles/minbleps.php
//
// See LICENSE.txt for license and usage information.
//

#ifndef TONIC_DSPUTILS_H
#define TONIC_DSPUTILS_H

#include "TonicCore.h"

namespace Tonic
{
  
// --------------- Complex Math ------------------------
  
  //! Complex Absolute Value
  inline float cabs(float x, float y)
  {
    return sqrtf((x * x) + (y * y));
  }
  
  //! Complex Exponential
  inline void cexp(float x, float y, float *zx, float *zy)
  {
    float expx;
    
    expx = expf(x);
    *zx = expx * cosf(y);
    *zy = expx * sinf(y);
  }
  
// --------------- Window Generation -------------------
  
  //! Hann Window generation
  inline void GenerateHannWindow(int length, float *output)
  {
    int m = length - 1;
    int i;
    
    for (i=0; i <= m; i++)
    {
      output[i] = 0.5f*(1.f - cosf((2 * PI * i)/m));
    }
  }
  
  //! Hamming Window generation
  inline void GenerateHammingWindow(int length, float *output)
  {
    int m = length - 1;
    int i;
    
    for (i=0; i <= m; i++)
    {
      output[i] = 0.54f - 0.46f * cosf((2 * PI * i)/m);
    }
  }
  
  //! Blackman Window generation
  inline void GenerateBlackmanWindow(int length, float *output)
  {
    int m = length - 1;
    int i;
    float f1, f2, fm;
    
    fm = (float)m;
    for(i = 0; i <= m; i++)
    {
      f1 = (2.0f * PI * (float)i) / fm;
      f2 = 2.0f * f1;
      output[i] = 0.42f - (0.5f * cosf(f1)) + (0.08f * cosf(f2));
    }
  }
  
  // --------------- Time/Frequency Analysis --------------
  
  //! Discrete Fourier Transform
  /*!
      Non-FFT, brute force approach intended for wavetable generation.
      Do not use for real-time processing.
   */
  void DFT(int length, float *realTimeIn, float *imagTimeIn, float *realFreqOut, float *imagFreqOut);
  
  //! Inverse Discrete Fourier Transform
  /*!
      Non-FFT, brute force approach intended for wavetable generation.
      Do not use for real-time processing.
   */
  void InverseDFT(int length, float *realFreqIn, float *imagFreqIn, float *realTimeOut, float *imagTimeOut);
  
  //! Real Cepstrum
  void RealCepstrum(int length, float *signalIn, float *realCepstrumOut);

  //! Compute Minimum Phase Reconstruction of singal from its real cepstrum
  void MinimumPhase(int n, float *realCepstrum, float *minimumPhase);

  // ---------------- minBLEP Generation --------------------

  //! Generate minBlep
  /*!
      Returns pointer to buffer of length *lengthOut.
      Caller is responsible for freeing heap-allocated buffer.
   */
  float *GenerateMinBLEP(int zeroCrossings, int overSampling);

}


#endif /* defined(__TonicLib__DSPUtils__) */
