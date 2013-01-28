//
//  Multiplier.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 1/25/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#include "Multiplier.h"

namespace Tonic { namespace Tonic_{
  
  Multiplier_::Multiplier_(){
    workSpace.resize(0);
  }
  
  void Multiplier_::in(Generator& inputSource){
    inputs.push_back(inputSource);
  }

  
  void Multiplier_::tick( TonicFrames& frames){
  
    if (workSpace.frames() == 0) {
      workSpace.resize(frames.frames(), frames.channels());
    }

    memset(&frames[0], 0, sizeof(TonicFloat) * frames.size());
    
    // for the first generator, store the value in the frame argument
    inputs[0].tick(frames);
    
    // for additional generators, use the workspace stkframes for tick, and multiply it into the frames argument
    for(int i = 1; i < inputs.size(); i++) {
        inputs[i].tick(workSpace);
        float* framesPointer = &frames[0];
        float* workspacePointer = &workSpace[0];
        
        
        #ifdef USE_APPLE_ACCELERATE
            vDSP_vmul (
               framesPointer,       // __vDSP_input1[],
               1,                   //__vDSP_stride1,
               workspacePointer,    //const float __vDSP_input2[],
               1,                   //__vDSP_stride2,
               framesPointer,       //float __vDSP_result[],
               1,                   //vDSP_Stride __vDSP_strideResult,
               frames.frames() * frames.channels() //vDSP_Length __vDSP_size
            );
        #else
            for (int sampleCount = 0; sampleCount < frames.frames() * frames.channels(); sampleCount++) {
                *(framesPointer++) *= *(workspacePointer++);
            }
        #endif
        

    }

  }
  
}}
