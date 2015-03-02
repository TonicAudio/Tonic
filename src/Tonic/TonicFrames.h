//
//  TonicFrames.h
//  Tonic
//
//  Created by Morgan Packard on 1/23/13.
//
//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_TONICFRAMES_H
#define TONIC_TONICFRAMES_H

#include "TonicCore.h"
#include <sstream>

/*
  This is an almost exact copy of STKFrames. Many thanks to Perry Cook and Gary Scavone.
  https://ccrma.stanford.edu/software/stk/
*/

namespace Tonic {


  class TonicFrames
  {
  public:

    //! The default constructor initializes the frame data structure to size zero.
    TonicFrames( unsigned int nFrames = 0, unsigned int nChannels = 0 );

    //! Overloaded constructor that initializes the frame data to the specified size with \c value.
    TonicFrames( const TonicFloat& value, unsigned int nFrames, unsigned int nChannels );

    //! The destructor.
    virtual ~TonicFrames();

    // A copy constructor.
    TonicFrames( const TonicFrames& f );

    // Assignment operator that returns a reference to self.
    TonicFrames& operator= ( const TonicFrames& f );

    //! Subscript operator that returns a reference to element \c n of self.
    /*!
      The result can be used as an lvalue. This reference is valid
      until the resize function is called or the array is destroyed. The
      index \c n must be between 0 and size less one.  No range checking
      is performed unless TONIC_DEBUG is defined.
    */
    TonicFloat& operator[] ( size_t n );

    //! Subscript operator that returns the value at element \c n of self.
    /*!
      The index \c n must be between 0 and size less one.  No range
      checking is performed unless TONIC_DEBUG is defined.
    */
    TonicFloat operator[] ( size_t n ) const;

    //! Assignment by sum operator into self.
    /*!
      The dimensions of the argument are expected to be the same as
      self.  No range checking is performed unless TONIC_DEBUG is
      defined.
    */
    void operator+= ( TonicFrames& f );
    
    
    void operator-= ( TonicFrames& f );

    //! Assignment by product operator into self.
    /*!
      The dimensions of the argument are expected to be the same as
      self.  No range checking is performed unless TONIC_DEBUG is
      defined.
    */
    void operator*= ( TonicFrames& f );
    
    
    void operator/= ( TonicFrames& f );

    //! Channel / frame subscript operator that returns a reference.
    /*!
      The result can be used as an lvalue. This reference is valid
      until the resize function is called or the array is destroyed. The
      \c frame index must be between 0 and frames() - 1.  The \c channel
      index must be between 0 and channels() - 1.  No range checking is
      performed unless TONIC_DEBUG is defined.
    */
    TonicFloat& operator() ( size_t frame, unsigned int channel );

    //! Channel / frame subscript operator that returns a value.
    /*!
      The \c frame index must be between 0 and frames() - 1.  The \c
      channel index must be between 0 and channels() - 1.  No range checking
      is performed unless TONIC_DEBUG is defined.
    */
    TonicFloat operator() ( size_t frame, unsigned int channel ) const;
      
      
    //! Copy one channel to another
    /*!
      The \c src and \c dst indices must be between 0 and channels() - 1, and
      should not be the same number. No range checking is performed (yet)
    */
    void copyChannel(unsigned int src, unsigned int dst);

    //! Fill all channels with contents of channel 0
    void fillChannels();
    
    //! clear the frames data
    void clear();
    
    //! Fill frames from other source.
    /*! 
      Copies channels from one object to another. Frame count must match.
      If source has more channels than destination, they will be averaged.
      If destination has more channels than source, they will be copied to all channels.
    */
    void copy( TonicFrames & f );
        
    //! Return an interpolated value at the fractional frame index and channel.
    /*!
      This function performs linear interpolation.  The \c frame
      index must be between 0.0 and frames() - 1.  The \c channel index
      must be between 0 and channels() - 1.  No range checking is
      performed unless TONIC_DEBUG is defined.
    */
    TonicFloat interpolate( TonicFloat frame, unsigned int channel = 0 ) const;

    //! Returns the total number of audio samples represented by the object.
    size_t size() const { return size_; }; 

    //! Returns \e true if the object size is zero and \e false otherwise.
    bool empty() const;

    //! Resize self to represent the specified number of channels and frames.
    /*!
      Changes the size of self based on the number of frames and
      channels.  No element assignment is performed.  No memory
      deallocation occurs if the new size is smaller than the previous
      size.  Further, no new memory is allocated when the new size is
      smaller or equal to a previously allocated size.
    */
    void resize( size_t nFrames, unsigned int nChannels = 1 );


    //! Resize self to represent the specified number of channels and frames and perform element initialization.
    /*!
      Changes the size of self based on the number of frames and
      channels, and assigns \c value to every element.  No memory
      deallocation occurs if the new size is smaller than the previous
      size.  Further, no new memory is allocated when the new size is
      smaller or equal to a previously allocated size.
    */
    void resize( size_t nFrames, unsigned int nChannels, TonicFloat value );
    
    //! Resize and stretch/shrink existing data to fit new size.
    void resample( size_t nFrames , unsigned int nChannels );

    //! Return the number of channels represented by the data.
    inline unsigned int channels( void ) const { return nChannels_; };

    //! Return the number of sample frames represented by the data.
    inline unsigned long frames( void ) const { return nFrames_; };

    //! Set the sample rate associated with the TonicFrames data.
    /*!
      By default, this value is set equal to the current STK sample
      rate at the time of instantiation.
     */
    void setDataRate( TonicFloat rate ) { dataRate_ = rate; };

    //! Return the sample rate associated with the TonicFrames data.
    /*!
      By default, this value is set equal to the current STK sample
      rate at the time of instantiation.
     */
    TonicFloat dataRate( void ) const { return dataRate_; };

  protected:

    TonicFloat *data_;
    TonicFloat dataRate_;
    size_t nFrames_;
    unsigned int nChannels_;
    size_t size_;
    size_t bufferSize_;

  };

  inline bool TonicFrames :: empty() const
  {
    if ( size_ > 0 ) return false;
    else return true;
  }

  inline TonicFloat& TonicFrames :: operator[] ( size_t n )
  {
  #if defined(TONIC_DEBUG)
    if ( n >= size_ ) {
      std::ostringstream error;
      error << "TonicFrames::operator[]: invalid index (" << n << ") value!";
      Tonic::error(error.str(), true);
    }
  #endif

    return data_[n];
  }

  inline TonicFloat TonicFrames :: operator[] ( size_t n ) const
  {
  #if defined(TONIC_DEBUG)
    if ( n >= size_ ) {
      std::ostringstream error;
      error << "TonicFrames::operator[]: invalid index (" << n << ") value!";
      Tonic::error(error.str(), true);
    }
  #endif

    return data_[n];
  }

  inline TonicFloat& TonicFrames :: operator() ( size_t frame, unsigned int channel )
  {
  #if defined(TONIC_DEBUG)
    if ( frame >= nFrames_ || channel >= nChannels_ ) {
      std::ostringstream error;
      error << "TonicFrames::operator(): invalid frame (" << frame << ") or channel (" << channel << ") value!";
      Tonic::error(error.str(), true);
    }
  #endif

    return data_[ frame * nChannels_ + channel ];
  }

  inline TonicFloat TonicFrames :: operator() ( size_t frame, unsigned int channel ) const
  {
  #if defined(TONIC_DEBUG)
    if ( frame >= nFrames_ || channel >= nChannels_ ) {
      std::ostringstream error;
      error << "TonicFrames::operator(): invalid frame (" << frame << ") or channel (" << channel << ") value!";
      Tonic::error(error.str(), true);
    }
  #endif

    return data_[ frame * nChannels_ + channel ];
  }
    
  inline void TonicFrames :: copyChannel(unsigned int src, unsigned int dst)
  {

    TonicFloat *sptr = data_ + src;
    TonicFloat *dptr = data_ + dst;
    unsigned int stride = nChannels_;
    
    vcopy(dptr, stride, sptr, stride, (unsigned int)nFrames_);

  }

  inline void TonicFrames::fillChannels()
  {
    for (unsigned int i=1; i<nChannels_; i++){
      this->copyChannel(0, i);
    }
  }
  
  inline void TonicFrames::clear(){
    memset(data_, 0, size_ * sizeof(TonicFloat));
  }
  
  inline void TonicFrames::copy( TonicFrames &f ){
    
#if defined(TONIC_DEBUG)
    if ( f.frames() != nFrames_) {
      std::ostringstream error;
      error << "TonicFrames::operator+=: frames argument must be of equal dimensions!";
      Tonic::error(error.str(), true);
    }
#endif
    
    unsigned int fChannels = f.channels();
    TonicFloat *dptr = data_;
    TonicFloat *fptr = &f[0];
    
    if (nChannels_ == fChannels){
      memcpy(dptr, fptr, size_ * sizeof(TonicFloat));
    }
    else if (nChannels_ < fChannels){
      
      // sum channels
      memset(dptr, 0, size_ * sizeof(TonicFloat));
      for (unsigned int c=0; c<fChannels; c++){
        dptr = data_;
        fptr = &f(0,c);
        for (unsigned int i=0; i<nFrames_; i++, dptr+=nChannels_, fptr+=fChannels){
          *dptr += *fptr;
        }
      }
      
      // apply scaling (average of channels)
      TonicFloat s = 1.0f/fChannels;
      dptr = data_;
      for (unsigned int i=0; i<nFrames_; i++, dptr+=nChannels_){
        *dptr *= s;
      }
    }
    else{
      // just copy one channel, then fill
      vcopy(dptr, nChannels_, fptr, fChannels, (unsigned int)nFrames_);
      
      // fill all channels if necessary
      fillChannels();
    }
      
  }

  inline void TonicFrames :: operator+= ( TonicFrames& f )
  {
  #if defined(TONIC_DEBUG)
    if ( f.frames() != nFrames_ ) {
      std::ostringstream error;
      error << "TonicFrames::operator+=: frames argument must be of equal dimensions!";
      Tonic::error(error.str(), true);
    }
  #endif
    
    TonicFloat *fptr = &f[0];
    TonicFloat *dptr = data_;
    
    unsigned int fChannels = f.channels();
    
    if (nChannels_ == fChannels){
      
#ifdef USE_APPLE_ACCELERATE
      vDSP_vadd(dptr, 1, fptr, 1, dptr, 1, size_);
#else
      for ( unsigned int i=0; i<size_; i++ )
        *dptr++ += *fptr++;
#endif
      
    }
    else if (nChannels_ < fChannels){
      
      //  just add first channel of rhs
#ifdef USE_APPLE_ACCELERATE
      vDSP_vadd(dptr, 1, fptr, fChannels, dptr, 1, nFrames_);
#else
      for ( unsigned int i=0; i<nFrames_; i++ ){
        *dptr++ -= *fptr++;
        fptr++;
      }
#endif
      
    }
    else{
      //  add rhs to both channels
#ifdef USE_APPLE_ACCELERATE
      vDSP_vadd(dptr, 2, fptr, 1, dptr, 2, nFrames_);
      vDSP_vadd(dptr+1, 2, fptr, 1, dptr+1, 2, nFrames_);
#else
      for ( unsigned int i=0; i<nFrames_; i++ ){
        *dptr++ += *fptr;
        *dptr++ += *fptr++;
      }
#endif
    }
    
  }
  
  
  inline void TonicFrames :: operator -= ( TonicFrames& f )
  {
  #if defined(TONIC_DEBUG)
    if ( f.frames() != nFrames_ ) {
      std::ostringstream error;
      error << "TonicFrames::operator+=: frames argument must be of equal dimensions!";
      Tonic::error(error.str(), true);
    }
  #endif

    
    TonicFloat *fptr = &f[0];
    TonicFloat *dptr = data_;

    unsigned int fChannels = f.channels();
    
    if (nChannels_ == fChannels){
      
#ifdef USE_APPLE_ACCELERATE
      vDSP_vsub(fptr, 1, dptr, 1, dptr, 1, size_);
#else
      for ( unsigned int i=0; i<size_; i++ )
        *dptr++ -= *fptr++;
#endif
    }
    else if (nChannels_ < fChannels){
      
      //  just subtract first channel of rhs
#ifdef USE_APPLE_ACCELERATE
      vDSP_vsub(dptr, 1, fptr, fChannels, dptr, 1, nFrames_);
#else
      for ( unsigned int i=0; i<nFrames_; i++ ){
        *dptr++ -= *fptr++;
        fptr++;
      }
#endif
      
    }
    else{
      //  subtract both channels by rhs
#ifdef USE_APPLE_ACCELERATE
      vDSP_vsub(dptr, 2, fptr, 1, dptr, 2, nFrames_);
      vDSP_vsub(dptr+1, 2, fptr, 1, dptr+1, 2, nFrames_);
#else
      for ( unsigned int i=0; i<nFrames_; i++ ){
        *dptr++ -= *fptr;
        *dptr++ -= *fptr++;
      }
#endif
    }


  }
  

  inline void TonicFrames :: operator*= ( TonicFrames& f )
  {
    
#if defined(TONIC_DEBUG)
    if ( f.frames() != nFrames_) {
      std::ostringstream error;
      error << "TonicFrames::operator+=: frames argument must be of equal dimensions!";
      Tonic::error(error.str(), true);
    }
#endif
    
    TonicFloat *fptr = &f[0];
    TonicFloat *dptr = data_;

    unsigned int fChannels = f.channels();
    
    if (nChannels_ == fChannels){
      
#ifdef USE_APPLE_ACCELERATE
      vDSP_vmul(dptr, 1, fptr, 1, dptr, 1, size_);
#else
      for ( unsigned int i=0; i<size_; i++ ){
        *dptr++ *= *fptr++;
      }
#endif
      
    }
    else if (nChannels_ < fChannels){
      
      //  just multiply by first channel of rhs
#ifdef USE_APPLE_ACCELERATE
      vDSP_vmul(dptr, 1, fptr, fChannels, dptr, 1, nFrames_);
#else
      for ( unsigned int i=0; i<nFrames_; i++ ){
        *dptr++ *= *fptr++;
        fptr++;
      }
#endif

    }
    else{
      //  multiply both channels by rhs
#ifdef USE_APPLE_ACCELERATE
      vDSP_vmul(dptr, 2, fptr, 1, dptr, 2, nFrames_);
      vDSP_vmul(dptr+1, 2, fptr, 1, dptr+1, 2, nFrames_);
#else
      for ( unsigned int i=0; i<nFrames_; i++ ){
        *dptr++ *= *fptr;
        *dptr++ *= *fptr++;
      }
#endif
    }
  }


  inline void TonicFrames :: operator /= ( TonicFrames& f )
  {
  #if defined(TONIC_DEBUG)
    if ( f.frames() != nFrames_ ) {
      std::ostringstream error;
      error << "TonicFrames::operator*=: frames argument must be of equal dimensions!";
      Tonic::error(error.str(), true);
    }
  #endif
    
    TonicFloat *fptr = &f[0];
    TonicFloat *dptr = data_;
    
    unsigned int fChannels = f.channels();
    
    if (nChannels_ == fChannels){
      
#ifdef USE_APPLE_ACCELERATE
      vDSP_vdiv(fptr, 1, dptr, 1, dptr, 1, size_);
#else
      for ( unsigned int i=0; i<size_; i++ ){
        *dptr++ /= *fptr++;
      }
#endif
      
    }
    else if (nChannels_ < fChannels){
      
      //  just multiply by first channel of rhs
#ifdef USE_APPLE_ACCELERATE
      vDSP_vdiv(fptr, fChannels, dptr, 1, dptr, 1, nFrames_);
#else
      for ( unsigned int i=0; i<nFrames_; i++ ){
        *dptr++ /= *fptr++;
        fptr++;
      }
#endif
      
    }
    else{
      //  multiply both channels by rhs
#ifdef USE_APPLE_ACCELERATE
      vDSP_vdiv(fptr, 2, dptr, 1, dptr, 2, nFrames_);
      vDSP_vdiv(fptr, 2, dptr+1, 1, dptr+1, 2, nFrames_);
#else
      for ( unsigned int i=0; i<nFrames_; i++ ){
        *dptr++ /= *fptr;
        *dptr++ /= *fptr++;
      }
#endif
    }
  }
  
}

#endif
