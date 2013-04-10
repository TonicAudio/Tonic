//
//  DelayUtils.h
//  Tonic
//
//  Created by Nick Donaldson on 3/10/13.

//

#ifndef __Tonic_DelayUtils__
#define __Tonic_DelayUtils__

#include "TonicFrames.h"

namespace Tonic {
  
  //! Tonicframes subclass with ability to tick in and out. Allows random-access/multi-tap/etc.
  class DelayLine : public TonicFrames {
    
  public:
    
    //! Allocation parameters are binding. No post-allocation resizing or modifying channel layout (for now anyway).
    /*!
        Samples are interleaved if allocated with multiple channels.
    */
    DelayLine();
    
    //! MUST be called prior to usage
    void initialize(float initialDelay = 0.5f, float maxDelay = 1.0f, unsigned int channels = 1);
    
    //! Set whether interpolates or not
    void setInterpolates( bool doesInterpolate ) { interpolates_ = doesInterpolate; };
    
    //! Zero delay line
    void clear();
    
    // !!!: ND
    // The below functions are single-sample and very one-purposed for a reason:
    // as a helper class this will allow the most flexibility for feedback  and more complex
    // delay structures such as comb filters, etc.
    
    //! Return one interpolated, delayed sample. Does not advance read/write head.
    inline TonicFloat tickOut(unsigned int channel = 0) {
      
      if (interpolates_){
        // Fractional and integral part of read head
        float fidx;
        float frac = modf(readHead_, &fidx);
        int idx_a = ((int)fidx * nChannels_ + channel);
        int idx_b = idx_a + nChannels_;
        if (idx_b >= nFrames_) idx_b -= nFrames_;
        
        // Linear interpolation, for now. Would like to use allpass - better in general for delay lines.
        return (data_[idx_a] + frac * (data_[idx_b] - data_[idx_a]));
      }
      else{
        
        return (data_[((int)(readHead_))*nChannels_ + channel]);
      }
    }
    
    //! Tick one sample in (write at write head). Does not advance read/write head.
    inline void tickIn(TonicFloat sample, unsigned int channel = 0){
      
      data_[writeHead_*nChannels_+channel] = sample;
    }
    
    //! Advance read/write heads based on passed-in delay time.
    inline void advance(float delayTime){
      
      if (++writeHead_ >= nFrames_)
        writeHead_ = 0;
      
      if (delayTime != lastDelayTime_){
        float dSamp = clamp(delayTime * Tonic::sampleRate(), 0, nFrames_);
        readHead_ = (float)writeHead_ - dSamp;
        if (readHead_ < 0) {
          readHead_ += (float)nFrames_;
        }
        lastDelayTime_ = delayTime;
      }
      else{
        readHead_ += 1.0f;
        if (readHead_ >= nFrames_){
          readHead_ -= (float)nFrames_;
        }
      }
    }
    
  private:
    
    bool  isInitialized_;
    bool  interpolates_;
    
    long  writeHead_;
    float readHead_;
    float lastDelayTime_;
                     
  };
}

#endif
