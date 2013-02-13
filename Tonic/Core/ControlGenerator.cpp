//
//  ControlGenerator.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 2/12/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_ControlGenerator_cpp
#define TonicDemo_ControlGenerator_cpp

#include "ControlGenerator.h"

namespace Tonic{ namespace Tonic_{
  
  ControlGenerator_::ControlGenerator_(){
    pthread_mutex_init(&genMutex_, NULL);
  }

  ControlGenerator_::~ControlGenerator_(){
    pthread_mutex_destroy(&genMutex_);
  }

}}

#endif
