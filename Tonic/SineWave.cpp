//
//  SineWave.cpp
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/23/13.
//
//

#include "SineWave.h"

namespace Tonic {
  
TonicFrames SineWave_ :: table_;

SineWave_ :: SineWave_( void )
  : time_(0.0), rate_(1.0), phaseOffset_(0.0)
{
  if ( table_.empty() ) {
    table_.resize( TABLE_SIZE + 1, 1 );
    TonicFloat temp = 1.0 / TABLE_SIZE;
    for ( unsigned long i=0; i<=TABLE_SIZE; i++ )
      table_[i] = sin( TWO_PI * i * temp );
  }

}

SineWave_ :: ~SineWave_()
{
}

//void SineWave_ :: sampleRateChanged( TonicFloat newRate, TonicFloat oldRate )
//{
//  if ( !ignoreSampleRateChange_ )
//    this->setRate( oldRate * rate_ / newRate );
//}

void SineWave_ :: reset( void )
{
  time_ = 0.0;
  //lastFrame_[0] = 0;
}

SineWave_& SineWave_ :: setFrequency( TonicFloat frequency )
{
  // This is a looping frequency.
  this->setRate( TABLE_SIZE * frequency / Tonic::sampleRate() );
}

void SineWave_ :: addTime( TonicFloat time )
{
  // Add an absolute time in samples.
  time_ += time;
}

void SineWave_ :: addPhase( TonicFloat phase )
{
  // Add a time in cycles (one cycle = TABLE_SIZE).
  time_ += TABLE_SIZE * phase;
}

void SineWave_ :: addPhaseOffset( TonicFloat phaseOffset )
{
  // Add a phase offset relative to any previous offset value.
  time_ += ( phaseOffset - phaseOffset_ ) * TABLE_SIZE;
  phaseOffset_ = phaseOffset;
}

}// end namespace
  
 