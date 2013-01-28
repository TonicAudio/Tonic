//
//  MonoToStereo.cpp
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/24/13.
//
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
