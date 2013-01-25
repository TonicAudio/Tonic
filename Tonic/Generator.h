//
//  Generator.h
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/23/13.
//
//

#ifndef ___013_1_23_melody__Generator__
#define ___013_1_23_melody__Generator__

#include <iostream>
#include "TonicFrames.h"

namespace Tonic {


class Generator {
  

public:

  virtual void tick( TonicFrames& frames) = 0;
  
protected:

};

  
}

#endif /* defined(___013_1_23_melody__Generator__) */
