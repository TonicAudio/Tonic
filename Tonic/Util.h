//
//  Util.h
//  TonicDemo
//
//  Created by Morgan Packard on 1/29/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_Util_h
#define TonicDemo_Util_h

#include <algorithm>

namespace Tonic {
  class Util{
  public:
    static float randFloat(float x, float y){
    
    	float high = 0;
      float low = 0;
      float randNum = 0;
      // if there is no range, return the value
      if (x == y) return x; 			// float == ?, wise? epsilon?
      high = std::max(x,y);
      low = std::min(x,y);
      randNum = low + ((high-low) * rand()/(RAND_MAX + 1.0));
      return randNum;
    }
  };
}

#endif
