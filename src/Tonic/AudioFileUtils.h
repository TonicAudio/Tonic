//
//  AudioFileUtils.h
//  TonicLib
//
//  Created by Morgan Packard on 10/26/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#ifndef __TonicLib__AudioFileUtils__
#define __TonicLib__AudioFileUtils__

#include "SampleTable.h"

namespace Tonic {
  
  SampleTable loadAudioFile(string path, int numChannels = 2);
  
}

#endif /* defined(__TonicLib__AudioFileUtils__) */
