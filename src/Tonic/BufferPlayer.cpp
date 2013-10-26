//
//  BufferPlayer.cpp
//  Tonic
//
//  Created by Morgan Packard on 10/26/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "BufferPlayer.h"

namespace Tonic { namespace Tonic_{
  
  BufferPlayer_::BufferPlayer_() : currentSample(0){
    
  }
  
  BufferPlayer_::~BufferPlayer_(){
    
  }
  
  void  BufferPlayer_::setBuffer(SampleTable buffer){
    buffer_ = buffer;
    setIsStereoOutput(buffer.channels() == 2);
    
  }

} // Namespace Tonic_
  
  
  
} // Namespace Tonic
