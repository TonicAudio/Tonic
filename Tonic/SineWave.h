//
//  SineWave.h
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/23/13.
//
//

/*
  For now, this is a line-for-line copy of STK SineWave_. Much thanks to Perry Cook and Gary Scavone. 
  https://ccrma.stanford.edu/software/stk/
*/

#ifndef ___013_1_23_melody__SineWave___
#define ___013_1_23_melody__SineWave___

#include <iostream>
#include "Tonic.h"
#include "Generator.h"

namespace Tonic {


  const unsigned long TABLE_SIZE = 2048;

  class SineWave_ : public Generator_
  {
  public:
    //! Default constructor.
    SineWave_( void );

    //! Class destructor.
    ~SineWave_( void );

    //! Clear output and reset time pointer to zero.
    void reset( void );

    //! Set the data read rate in samples.  The rate can be negative.
    /*!
      If the rate value is negative, the data is read in reverse order.
    */
    inline void setRate( TonicFloat rate ) { rate_ = rate; };

    //! Set the data interpolation rate based on a looping frequency.
    /*!
      This function determines the interpolation rate based on the file
      size and the current Stk::sampleRate.  The \e frequency value
      corresponds to file cycles per second.  The frequency can be
      negative, in which case the loop is read in reverse order.
     */
    SineWave_& setFrequency( TonicFloat frequency );

    //! Increment the read pointer by \e time in samples, modulo the table size.
    void addTime( TonicFloat time );

    //! Increment the read pointer by a normalized \e phase value.
    /*!
      This function increments the read pointer by a normalized phase
      value, such that \e phase = 1.0 corresponds to a 360 degree phase
      shift.  Positive or negative values are possible.
     */
    void addPhase( TonicFloat phase );

    //! Add a normalized phase offset to the read pointer.
    /*!
      A \e phaseOffset = 1.0 corresponds to a 360 degree phase
      offset.  Positive or negative values are possible.
     */
    void addPhaseOffset( TonicFloat phaseOffset );

    //! Compute and return one output sample.
    TonicFloat tick( void );

    //! Fill a channel of the TonicFrames object with computed outputs.
    /*!
      The \c channel argument must be less than the number of
      channels in the TonicFrames argument (the first channel is specified
      by 0).  However, range checking is only performed if _STK_DEBUG_
      is defined during compilation, in which case an out-of-range value
      will trigger an StkError exception.
    */
    inline void tick( TonicFrames& frames );

  protected:
//
//    void sampleRateChanged( TonicFloat newRate, TonicFloat oldRate );

    static TonicFrames table_;
    TonicFloat time_;
    TonicFloat rate_;
    TonicFloat phaseOffset_;
    unsigned int iIndex_;
    TonicFloat alpha_;

  };

  inline void SineWave_ :: tick( TonicFrames& frames )
  {
  #if defined(_STK_DEBUG_)
    if ( channel >= frames.channels() ) {
      oStream_ << "SineWave_::tick(): channel and TonicFrames arguments are incompatible!";
      handleError( StkError::FUNCTION_ARGUMENT );
    }
  #endif

  

    TonicFloat *samples = &frames[0];
    TonicFloat tmp = 0.0;

    unsigned int hop = frames.channels();
    for ( unsigned int i=0; i<frames.frames(); i++, samples += hop ) {

      // Check limits of time address ... if necessary, recalculate modulo
      // TABLE_SIZE.
      while ( time_ < 0.0 )
        time_ += TABLE_SIZE;
      while ( time_ >= TABLE_SIZE )
        time_ -= TABLE_SIZE;

      iIndex_ = (unsigned int) time_;
      alpha_ = time_ - iIndex_;
      tmp = table_[ iIndex_ ];
      tmp += ( alpha_ * ( table_[ iIndex_ + 1 ] - tmp ) );
      *samples = tmp;

      // Increment time, which can be negative.
      time_ += rate_;
    }

  }

  class SineWave : public TemplatedGenerator<SineWave_>{
  public:
    SineWave& setFrequency(float freq){
      gen()->setFrequency(freq);
      return *this;
    }
  };

}

#endif /* defined(___013_1_23_melody__SineWave___) */
