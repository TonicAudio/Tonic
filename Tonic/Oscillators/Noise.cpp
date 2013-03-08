//
//  Noise.h
//  Tonic
//
//  Created by Nick Donaldson on 2/7/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

/*+++++++++++++++++++++ License ++++++++++++++++++++
 
 Use this code for whatever you want. There are NO
 RESTRICTIONS WHATSOVER. Modify it, repackage it,
 sell it, get rich from it, whatever. Go crazy. If
 you want to make mehappy, contribute to this
 project, but feel free to just use the code as a
 starting point for whatever you like.
 
 Note that Tonic is heavily indebted to STK
 https://ccrma.stanford.edu/software/stk/
 
 ++++++++++++++++++++++++++++++++++++++++++++++++++*/

#import "Noise.h"

namespace Tonic {
  namespace Tonic_ {
  
    PinkNoise_::PinkNoise_() : pinkCount_(1), pinkAccum_(0) {
      memset(pinkBins_, 0, kNumPinkNoiseBins * sizeof(TonicFloat));
    }
  
  }
}