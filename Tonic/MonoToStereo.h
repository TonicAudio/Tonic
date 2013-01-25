//
//  MonoToStereo.h
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/24/13.
//
//

#ifndef ___013_1_23_melody__MonoToStereo__
#define ___013_1_23_melody__MonoToStereo__

#include <iostream>
#include "Processor.h"

namespace Tonic {

class MonoToStereo : public Processor{

  TonicFrames         mInputFrames;

public:

  MonoToStereo();

  void                tick( TonicFrames& frames );
  
  
};

}

#endif /* defined(___013_1_23_melody__MonoToStereo__) */
