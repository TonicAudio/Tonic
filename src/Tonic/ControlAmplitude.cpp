//
//  ControlAmplitude.cpp
//  Tonic
//
//

#include "ControlAmplitude.h"

namespace Tonic {
  namespace Tonic_{

    void ControlAmplitude_::setInput(Tonic::Generator input){
      inputFrames.resize(kSynthesisBlockSize, input.isStereoOutput() ? 2 : 1);
      input_ = input;
    }

    void ControlAmplitude_::computeOutput(const SynthesisContext_ &context)
    {
      int inputChannelCount = input_.isStereoOutput() ? 2 : 1;
      if (inputFrames.channels() != inputChannelCount ) {
        inputFrames.resize(kSynthesisBlockSize, 1);
      }
      input_.tick(inputFrames, context);
      TonicFloat total = 0;
      TonicFloat* data = &inputFrames[0];
      for (int i = 0; i < inputFrames.channels() * kSynthesisBlockSize; i++)
      {
        total +=  fabs( *data );
        data++;
      }
      output_.value = total / (inputFrames.channels() * kSynthesisBlockSize);
    }

    ControlAmplitude_::ControlAmplitude_()
    {
      inputFrames.resize(kSynthesisBlockSize, 1);
    }

    ControlAmplitude_::~ControlAmplitude_()
    {

    }

  }

} // Namespace Tonic
