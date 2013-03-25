//
//  StereoDelay.cpp
//  Tonic
//
//  Created by Morgan Packard on 3/13/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "StereoDelay.h"

namespace Tonic { namespace Tonic_{
  
  StereoDelay_::StereoDelay_(){
    setIsStereoOutput(true);
  }
  
  void StereoDelay_::initialize(float leftDelayArg, float rightDelayArg, float maxDelayLeft, float maxDelayRight){
    maxDelayLeft = maxDelayLeft > 0? maxDelayLeft : 2 * leftDelayArg;
    maxDelayRight = maxDelayRight > 0? maxDelayRight : 2 * rightDelayArg;
    leftDelay = MonoDelay(maxDelayLeft).delayTime(leftDelayArg).mix(0.5).feedback(0.5);
    rightDelay = MonoDelay(maxDelayRight).delayTime(rightDelayArg).mix(0.5).feedback(0.5);
    outputGen = leftPanner.pan(-1).input(leftDelay) + rightPanner.input(rightDelay).pan(1);
  }
  
  StereoDelay_::~StereoDelay_(){
    
  }
  
  void StereoDelay_::setInput(Generator inputArg){
    leftDelay.input(inputArg);
    rightDelay.input(inputArg);
  }
  
  
  void StereoDelay_::setMix(Generator arg){
    leftDelay.mix(arg);
    rightDelay.mix(arg);
  };
  void StereoDelay_::setFeedback(Generator arg){
    leftDelay.feedback(arg);
    rightDelay.feedback(arg);
  };
  void StereoDelay_::setDelayTimeLeft(Generator arg){
    leftDelay.delayTime(arg);
  };
  void StereoDelay_::setDelayTimeRight(Generator arg){
    rightDelay.delayTime(arg);
  };
  
  
} // Namespace Tonic_
  
  
  StereoDelay:: StereoDelay(float leftDelay, float rightDelay, float maxDelayLeft, float maxDelayRight){
    gen()->initialize(leftDelay, rightDelay, maxDelayLeft, maxDelayRight);
  }
  
} // Namespace Tonic
