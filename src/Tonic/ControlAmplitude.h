//
//  ControlAmplitude.h
//  Tonic 
//
//  Created by Morgan Packard on 1/14/15.
//
// See LICENSE.txt for license and usage information.
//

/*
  Input is an audio-rate Generator. Output is a control-rate signal representing the average absolute-value amplitude of the audio signal for the current synthesis block. 
  This is what you would use if you wanted to make a level meter, or modulate a parameter with the amplitude of a signal. 
  It would be more accurate to call it ControlMagnitude, since it's absolute value, not -1 to 1, but names need to be chosen, decisions need to be made,
  and hindsight is 20/20.
*/

#ifndef TONIC_CONTROLAMPLITUDE_H
#define TONIC_CONTROLAMPLITUDE_H

#include "ControlGenerator.h"
#include "Generator.h"
#include "TonicFrames.h"

namespace Tonic {

  namespace Tonic_ {

    class ControlAmplitude_ : public ControlGenerator_{

    protected:
      Generator input_;
      Tonic::TonicFrames inputFrames; // not sure why I needed to specify Tonic:: namespace here
  
    public:
      ControlAmplitude_();
      ~ControlAmplitude_();
      void setInput(Generator input){ input_ = input; };
      void computeOutput(const SynthesisContext_ &context);
    };
  }

  class ControlAmplitude : public TemplatedControlGenerator<Tonic_::ControlAmplitude_>{

  protected:


  public:
    TONIC_MAKE_GEN_SETTERS(ControlAmplitude, input, setInput)
  };
}

#endif


