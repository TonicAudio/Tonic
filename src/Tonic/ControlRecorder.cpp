//
//  ControlRecorder.cpp
//  Tonic
//
//  Created by Morgan Packard on 4/11/13.
//
//

#include "ControlRecorder.h"

namespace Tonic { namespace Tonic_{
  
  ControlRecorder_::ControlRecorder_() : playbackHead(recording.begin()){
    
  }
  
  ControlRecorder_::~ControlRecorder_(){
    
  }
  
  void ControlRecorder_::computeOutput(const SynthesisContext_ & context){
    ControlGeneratorOutput inputOut = input_.tick(context);
    ControlGeneratorOutput modeOut = mode.tick(context);
    
    ControlRecorder::Mode currenMode = (ControlRecorder::Mode)modeOut.value;
    
    if(modeOut.status == ControlGeneratorStatusHasChanged){
      if(currenMode == ControlRecorder::STOP){
        printf("ControlRecorder_::computeOutput STOP\n");
        recording.clear();
      }else if(currenMode == ControlRecorder::PLAY){
        playbackHead = recording.begin();
      }else if(currenMode == ControlRecorder::RECORD){
        playbackHead = recording.begin();
        recording.clear();
      }
    }
    
    // temp
    static int count = 0;
    
    switch (currenMode) {
      case ControlRecorder::RECORD:
        recording.push_back(inputOut);
        lastOutput_ = inputOut;
        break;

      case ControlRecorder::STOP:
        lastOutput_ = inputOut;
        break;

      case ControlRecorder::PLAY:
        lastOutput_ = *playbackHead;
        playbackHead++;
        count++;
        if (playbackHead >= recording.end()) {
          playbackHead = recording.begin();
          count = 0;
        }
//        printf("ControlRecorder_::computeOutput playing back sample: %i of %lu. Value is: %f\n", count, recording.size(), lastOutput_.value);
        break;
        
      default:
        break;
    }
    
  }
  
  
  void  ControlRecorder_::setMode(ControlGenerator modeArg){
    mode = modeArg;
  }
  
  
  
} // Namespace Tonic_
  
  
  
} // Namespace Tonic
