//
//  MonoToStereo.cpp
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/24/13.
//
//

#include "MonoToStereo.h"

namespace Tonic {

  
//MonoToStereo::MonoToStereo( Generator& input ) : mInputFrames(TonicFrames()){
//  mInputSource = &input;
//  mInputFrames.resize(kSynthesisBlockSize, 1);
//}
//  
//void  MonoToStereo::tick( TonicFrames& frames ){
//  mInputSource->tick(mInputFrames);
//  TonicFloat* sourceData = &mInputFrames[0];
//  TonicFloat* destData = &frames[0];
//  for(int i = 0; i < mInputFrames.size(); i++){
//      *(destData++) = *sourceData;
//      *(destData++) = *(sourceData++);
//  }
//}

}
