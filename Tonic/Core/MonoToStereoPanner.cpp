//
//  MonoToStereoPanner.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 3/7/13.

//

#include "MonoToStereoPanner.h"

namespace Tonic{

  namespace Tonic_{
  
    MonoToStereoPanner_::MonoToStereoPanner_(){
      setIsStereoOutput(true);
      panFrames.resize(kSynthesisBlockSize, 1);
      setPan(ControlValue(0));
    }
    MonoToStereoPanner_::~MonoToStereoPanner_(){}
  
    void MonoToStereoPanner_::setPan(ControlGenerator panArg){
      panControlGen = panArg;
    }
  }

}
