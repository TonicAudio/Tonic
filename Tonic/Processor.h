//
//  Processor.h
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/24/13.
//
//

#ifndef ___013_1_23_melody__Processor__
#define ___013_1_23_melody__Processor__

#include <iostream>
#include "Generator.h"

namespace Tonic {

class Processor : public Generator{

protected:
  Generator*          mInputSource;

public:
  virtual void        in(Generator* inputSource);
};

}

#endif /* defined(___013_1_23_melody__Processor__) */
