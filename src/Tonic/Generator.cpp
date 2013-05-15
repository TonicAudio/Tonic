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
    pthread_mutex_init(&genMutex_, NULL);
    outputFrames_.resize(kSynthesisBlockSize, 1, 0);
  }

  Generator_::~Generator_(){
    pthread_mutex_destroy(&genMutex_);
  }
  
  void Generator_::setIsStereoOutput(bool stereo){
    if (stereo != isStereoOutput_){
      outputFrames_.resize(kSynthesisBlockSize, stereo ? 2 : 1, 0);
    }
    isStereoOutput_ = stereo;
  }

}}
