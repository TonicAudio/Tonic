//
//  StereoDelay.h
//  Tonic 
//
//  Created by Morgan Packard on 3/13/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__StereoDelay__
#define __Tonic__StereoDelay__

#include "Generator.h"
#include "FixedValue.h"
#include "MonoToStereoPanner.h"
#include "BasicDelay.h"
#include "Adder.h"
#include "Effect.h"

namespace Tonic {
  
  namespace Tonic_ {

    class StereoDelay_ : public Effect_{
      
    protected:
    
      BasicDelay leftDelay;
      BasicDelay rightDelay;
      MonoToStereoPanner leftPanner;
      MonoToStereoPanner rightPanner;
      Generator outputGen;
      
    public:
      StereoDelay_();
      ~StereoDelay_();
      void computeSynthesisBlock( const SynthesisContext_ &context );
      void setInput(Generator);
      void setMix(Generator);
      void setFeedback(Generator);
      void setDelayTimeLeft(Generator);
      void setDelayTimeRight(Generator);
      void initialize(float leftDelay, float rightDelay, float maxDelayLeft, float maxDelayRight);
      
    };
    
    inline void StereoDelay_::computeSynthesisBlock(const SynthesisContext_ &context){
      outputGen.tick(synthesisBlock_, context);
    }
    
    
  }
  
  class StereoDelay : public TemplatedEffect<StereoDelay, Tonic_::StereoDelay_>{
    
  public:
  
  StereoDelay(float leftDelay, float rightDelay, float maxDelayLeft = -1, float maxDelayRight = -1);
  
  createGeneratorSetters(StereoDelay, mix, setMix)
  createGeneratorSetters(StereoDelay, feedback, setFeedback)
  createGeneratorSetters(StereoDelay, delayTimeLeft, setDelayTimeLeft)
  createGeneratorSetters(StereoDelay, delayTimeRight, setDelayTimeRight)

  };
}

#endif /* defined(__Tonic__StereoDelay__) */


