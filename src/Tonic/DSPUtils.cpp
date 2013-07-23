//
//  DSPUtils.cpp
//  Tonic
//
//  Created by Nick D on 7/21/13.
//
//  Largely adapted from:
// http://www.experimentalscene.com/articles/minbleps.php
//

#include "DSPUtils.h"

namespace Tonic
{
  // --------------- Time/Frequency Analysis --------------
  
  // Discrete Fourier Transform
  void DFT(int length, float *realTimeIn, float *imagTimeIn, float *realFreqOut, float *imagFreqOut)
  {
    
    int k, i;
    float sr, si, p;
    
    memset(realFreqOut, 0, sizeof(float) * length);
    memset(imagFreqOut, 0, sizeof(float) * length);
    
    for(k = 0; k < length; k++)
      for(i = 0; i < length; i++)
      {
        p = (2.0f * PI * (float)(k * i)) / length;
        sr = cosf(p);
        si = -sinf(p);
        realFreqOut[k] += (realTimeIn[i] * sr) - (imagTimeIn[i] * si);
        imagFreqOut[k] += (realTimeIn[i] * si) + (imagTimeIn[i] * sr);
      }
  }
  
  // Inverse Discrete Fourier Transform
  void InverseDFT(int length, float *realFreqIn, float *imagFreqIn, float *realTimeOut, float *imagTimeOut)
  {
    int k, i;
    float sr, si, p;
    
    memset(realTimeOut, 0, sizeof(float) * length);
    memset(imagTimeOut, 0, sizeof(float) * length);
    
    for(k = 0; k < length; k++)
    {
      for(i = 0; i < length; i++)
      {
        p = (2.0f * PI * (float)(k * i)) / length;
        sr = cosf(p);
        si = -sinf(p);
        realTimeOut[k] += (realFreqIn[i] * sr) + (imagFreqIn[i] * si);
        imagTimeOut[k] += (realFreqIn[i] * si) - (imagFreqIn[i] * sr);
      }
      realTimeOut[k] /= (float)length;
      imagTimeOut[k] /= (float)length;
    }
  }
  
  // Real Cepstrum
  void RealCepstrum(int length, float *signalIn, float *realCepstrumOut)
  {
    float *realTime, *imagTime, *realFreq, *imagFreq;
    int i;
    
    realTime = new float[length];
    imagTime = new float[length];
    realFreq = new float[length];
    imagFreq = new float[length];
    
    // Compose Complex FFT Input
    
    for(i = 0; i < length; i++)
    {
      realTime[i] = signalIn[i];
      imagTime[i] = 0.0f;
    }
    
    // Perform DFT
    
    DFT(length, realTime, imagTime, realFreq, imagFreq);
    
    // Calculate Log Of Absolute Value
    
    for(i = 0; i < length; i++)
    {
      realFreq[i] = logf(cabs(realFreq[i], imagFreq[i]));
      imagFreq[i] = 0.0f;
    }
    
    // Perform Inverse FFT
    
    InverseDFT(length, realFreq, imagFreq, realTime, imagTime);
    
    // Output Real Part Of FFT
    for(i = 0; i < length; i++)
      realCepstrumOut[i] = realTime[i];
    
    delete realTime;
    delete imagTime;
    delete realFreq;
    delete imagFreq;
  }
  
  // Compute Minimum Phase Reconstruction Of Signal
  void MinimumPhase(int length, float *realCepstrum, float *minimumPhase)
  {
    int i, nd2;
    float *realTime, *imagTime, *realFreq, *imagFreq;
    
    nd2 = length / 2;
    realTime = new float[length];
    imagTime = new float[length];
    realFreq = new float[length];
    imagFreq = new float[length];
    
    if((length % 2) == 1)
    {
      realTime[0] = realCepstrum[0];
      for(i = 1; i < nd2; i++)
        realTime[i] = 2.0f * realCepstrum[i];
      for(i = nd2; i < length; i++)
        realTime[i] = 0.0f;
    }
    else
    {
      realTime[0] = realCepstrum[0];
      for(i = 1; i < nd2; i++)
        realTime[i] = 2.0f * realCepstrum[i];
      realTime[nd2] = realCepstrum[nd2];
      for(i = nd2 + 1; i < length; i++)
        realTime[i] = 0.0f;
    }
    
    for(i = 0; i < length; i++)
      imagTime[i] = 0.0f;
    
    DFT(length, realTime, imagTime, realFreq, imagFreq);
    
    for(i = 0; i < length; i++)
      cexp(realFreq[i], imagFreq[i], &realFreq[i], &imagFreq[i]);
    
    InverseDFT(length, realFreq, imagFreq, realTime, imagTime);
    
    for(i = 0; i < length; i++)
      minimumPhase[i] = realTime[i];
    
    delete realTime;
    delete imagTime;
    delete realFreq;
    delete imagFreq;
  }
  
  // ---------------- minBLEP Generation --------------------
  
  //! Generate MinBLEP And Return It In An Array Of Floating Point Values
  /*! 
      This can be used to create a new minimum phase bandlimited step function
      to store for later use as a lookup table.
   
      Note that BLEPOscillator.cpp already has a runtime constant version of a BLEP
      for use with the BLEP-based oscillators.
   
   */
  float *GenerateMinBLEP(int zeroCrossings, int overSampling)
  {
    int i, n, m;
    float r, a, b;
    float *buffer1, *buffer2, *minBLEP;
    
    // use power-of-two for DFT/Cepstrum for potential future
    // speed boost
    n = (zeroCrossings * 2 * overSampling) + 1;
    m = n-1;
    
    buffer1 = new float[m];
    buffer2 = new float[m];
    
    // Generate Sinc
    
    a = (float)-zeroCrossings;
    b = (float)zeroCrossings;
    for(i = 0; i < m; i++)
    {
      r = ((float)i) / ((float)m);
      buffer1[i] = sinc(a + (r * (b - a)));
    }
    
    GenerateBlackmanWindow(m, buffer2);
    for(i = 0; i < m; i++)
    {
      buffer1[i] *= buffer2[i];
    }
        
    // Minimum Phase Reconstruction
    
    RealCepstrum(m, buffer1, buffer2);
    MinimumPhase(m, buffer2, buffer1);
    
    // Integrate Into MinBLEP
    
    minBLEP = new float[n];
    a = 0.0f;
    for(i = 0; i < m; i++)
    {
        a += buffer1[i];
        minBLEP[i] = a;
    }
    
    // copy next-to-last sample
    minBLEP[m] = minBLEP[m-1];
    
    // Normalize
    a = minBLEP[m];
    a = 1.0f / a;
    for(i = 0; i < n; i++)
    {
      minBLEP[i] *= a;
    }
        
    delete buffer1;
    delete buffer2;
    return minBLEP;
  }

}