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
    
    InverseDFT(length, realTime, imagTime, realFreq, imagFreq);
    
    // Output Real Part Of FFT
    for(i = 0; i < length; i++)
      realCepstrumOut[i] = realTime[i];
    
    delete realTime;
    delete imagTime;
    delete realFreq;
    delete imagFreq;
  }
}