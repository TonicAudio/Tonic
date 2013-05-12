//
//  TonicFrames.cpp
//  Tonic
//
//  Created by Morgan Packard on 1/23/13.
//
//
// See LICENSE.txt for license and usage information.
//


#include "TonicFrames.h"

namespace Tonic {
  

TonicFrames :: TonicFrames( unsigned int nFrames, unsigned int nChannels )
  : nFrames_( nFrames ), nChannels_( nChannels )
{
  if (nChannels_ > 2){
    error("Invalid number of channels. TonicFrames is limited to mono or stereo only (1 or 2 channels)", true);
  }
  
  size_ = nFrames_ * nChannels_;
  bufferSize_ = size_;

  if ( size_ > 0 ) {
    data_ = (TonicFloat *) calloc( size_, sizeof( TonicFloat ) );
#if defined(TONIC_DEBUG)
    if ( data_ == NULL ) {
      std::string error = "TonicFrames: memory allocation error in constructor!";
      Stk::handleError( error, StkError::MEMORY_ALLOCATION );
    }
#endif
  }
  else data_ = 0;

  dataRate_ = Tonic::sampleRate();
}

TonicFrames :: TonicFrames( const TonicFloat& value, unsigned int nFrames, unsigned int nChannels )
  : nFrames_( nFrames ), nChannels_( nChannels )
{
  if (nChannels_ > 2){
    error("Invalid number of channels. TonicFrames is limited to mono or stereo only (1 or 2 channels)", true);
  }
  
  size_ = nFrames_ * nChannels_;
  bufferSize_ = size_;
  if ( size_ > 0 ) {
    data_ = (TonicFloat *) malloc( size_ * sizeof( TonicFloat ) );
#if defined(TONIC_DEBUG)
    if ( data_ == NULL ) {
      std::string error = "TonicFrames: memory allocation error in constructor!";
      Stk::handleError( error, StkError::MEMORY_ALLOCATION );
    }
#endif
    for ( long i=0; i<(long)size_; i++ ) data_[i] = value;
  }
  else data_ = 0;

  dataRate_ = Tonic::sampleRate();
}

TonicFrames :: ~TonicFrames()
{
  if ( data_ ) free( data_ );
}

TonicFrames :: TonicFrames( const TonicFrames& f )
  : size_(0), bufferSize_(0)
{
  resize( f.frames(), f.channels() );
  dataRate_ = Tonic::sampleRate();
  for ( unsigned int i=0; i<size_; i++ ) data_[i] = f[i];
}

TonicFrames& TonicFrames :: operator= ( const TonicFrames& f )
{
  size_ = 0;
  bufferSize_ = 0;
  resize( f.frames(), f.channels() );
  dataRate_ = Tonic::sampleRate();
  for ( unsigned int i=0; i<size_; i++ ) data_[i] = f[i];
  return *this;
}

void TonicFrames :: resize( size_t nFrames, unsigned int nChannels )
{
  
  if (nChannels > 2){
    error("Invalid number of channels. TonicFrames is limited to mono or stereo only (1 or 2 channels)", true);
  }
  
  if (nFrames != nFrames_ || nChannels != nChannels_){
    
    nFrames_ = nFrames;
    nChannels_ = nChannels;

    size_ = nFrames_ * nChannels_;
    if ( size_ > bufferSize_ ) {
      if ( data_ ) free( data_ );
      data_ = (TonicFloat *) malloc( size_ * sizeof( TonicFloat ) );
#if defined(TONIC_DEBUG)
      if ( data_ == NULL ) {
        std::string error = "TonicFrames::resize: memory allocation error!";
        Stk::handleError( error, StkError::MEMORY_ALLOCATION );
      }
#endif
      bufferSize_ = size_;
    }
  }
}

void TonicFrames :: resize( size_t nFrames, unsigned int nChannels, TonicFloat value )
{
  this->resize( nFrames, nChannels );

#ifdef USE_APPLE_ACCELERATE
  vDSP_vfill(&value, data_, 1, size_);
#else
  for ( size_t i=0; i<size_; i++ ) data_[i] = value;
#endif
  
}

TonicFloat TonicFrames :: interpolate( TonicFloat frame, unsigned int channel ) const
{
#if defined(TONIC_DEBUG)
  if ( frame < 0.0 || frame > (TonicFloat) ( nFrames_ - 1 ) || channel >= nChannels_ ) {
    std::ostringstream error;
    error << "TonicFrames::interpolate: invalid frame (" << frame << ") or channel (" << channel << ") value!";
    Stk::handleError( error.str(), StkError::MEMORY_ACCESS );
  }
#endif

  size_t iIndex = ( size_t ) frame;                    // integer part of index
  TonicFloat output, alpha = frame - (TonicFloat) iIndex;  // fractional part of index

  iIndex = iIndex * nChannels_ + channel;
  output = data_[ iIndex ];
  if ( alpha > 0.0 )
    output += ( alpha * ( data_[ iIndex + nChannels_ ] - output ) );

  return output;
}


}


