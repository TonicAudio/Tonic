//
//  Generator.cpp
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/23/13.
//
//
// See LICENSE.txt for license and usage information.
//


#include "Generator.h"
#include "Multiplier.h"

namespace Tonic{ namespace Tonic_{
  
  Generator_::Generator_() : lastFrameIndex_(0), isStereoOutput_(false){
    TONIC_MUTEX_INIT(&genMutex_);
    outputFrames_.resize(kSynthesisBlockSize, 1, 0);
  }

  Generator_::~Generator_(){
    TONIC_MUTEX_DESTROY(&genMutex_);
  }
  
  void Generator_::setIsStereoOutput(bool stereo){
    if (stereo != isStereoOutput_){
      outputFrames_.resize(kSynthesisBlockSize, stereo ? 2 : 1, 0);
    }
    isStereoOutput_ = stereo;
  }

}}
