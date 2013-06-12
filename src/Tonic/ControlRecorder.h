//
//  ControlRecorder.h
//  Tonic 
//
//  Created by Morgan Packard on 4/11/13.
//
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ControlRecorder__
#define __Tonic__ControlRecorder__

#include "ControlConditioner.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlRecorder_ : public ControlConditioner_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
      vector<ControlGeneratorOutput> recording;
      vector<ControlGeneratorOutput>::iterator playbackHead;
      ControlGenerator mode;
      
    public:
      ControlRecorder_();
      ~ControlRecorder_();
      
      void setMode(ControlGenerator);

    };
    
  }
  
  class ControlRecorder :  public TemplatedControlConditioner<ControlRecorder, Tonic_::ControlRecorder_>{
    
  public:
    
    enum Mode{
      RECORD,
      PLAY,
      STOP
    };
    
    createControlGeneratorSetters(ControlRecorder, mode, setMode)

  };
  
  // put down here so we can use the enum
  namespace Tonic_ {
    
    inline void ControlRecorder_::computeOutput(const SynthesisContext_ & context){
      
      ControlGeneratorOutput inputOut = input_.tick(context);
      ControlGeneratorOutput modeOut = mode.tick(context);
      
      ControlRecorder::Mode currenMode = (ControlRecorder::Mode)modeOut.value;
      
      if(modeOut.triggered){
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
          output_ = inputOut;
          break;
          
        case ControlRecorder::STOP:
          output_ = inputOut;
          break;
          
        case ControlRecorder::PLAY:
          output_ = *playbackHead;
          playbackHead++;
          count++;
          if (playbackHead >= recording.end()) {
            playbackHead = recording.begin();
            count = 0;
          }
          //        printf("ControlRecorder_::computeOutput playing back sample: %i of %lu. Value is: %f\n", count, recording.size(), output_.value);
          break;
          
        default:
          break;
      }
      
    }

  }
}

#endif /* defined(__Tonic__ControlRecorder__) */


