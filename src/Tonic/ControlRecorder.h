//
//  ControlRecorder.h
//  Tonic 
//
//  Created by Morgan Packard on 4/11/13.
//
//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_CONTROLRECORDER_H
#define TONIC_CONTROLRECORDER_H

#include "ControlConditioner.h"

namespace Tonic {


  namespace Tonic_ {


    class ControlRecorder_ : public ControlConditioner_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
	  vector<ControlGeneratorOutput> recording;
	  int playbackHead;
      ControlGenerator mode;
	  int currentMode; // actuall a ControlRecorder::Mode, but difficult to declare it as such
      
    public:
      ControlRecorder_();
      
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
    
    TONIC_MAKE_CTRL_GEN_SETTERS(ControlRecorder, mode, setMode)

  };
  
  // put down here so we can use the enum
  namespace Tonic_ {
    
    inline void ControlRecorder_::computeOutput(const SynthesisContext_ & context){
      
      ControlGeneratorOutput inputOut = input_.tick(context);
      ControlGeneratorOutput modeOut = mode.tick(context);
     
      ControlRecorder::Mode newMode = (ControlRecorder::Mode)((int)modeOut.value);
      
      if(newMode!= currentMode){
		 currentMode = newMode;
        if(newMode == ControlRecorder::STOP){
          printf("ControlRecorder_::computeOutput STOP\n");
          recording.clear();
        }else if(newMode == ControlRecorder::PLAY){
          playbackHead = 0;
        }else if(newMode == ControlRecorder::RECORD){
          playbackHead =0;
          recording.clear();
        }
      }
      
      
      switch (newMode) {
        case ControlRecorder::RECORD:
          recording.push_back(inputOut);
          output_ = inputOut;
          break;
          
        case ControlRecorder::STOP:
          output_ = inputOut;
          break;
          
        case ControlRecorder::PLAY:
          output_ = recording[playbackHead];
          playbackHead++;
          if (playbackHead >= recording.size()) {
            playbackHead = 0;
          }
          //        printf("ControlRecorder_::computeOutput playing back sample: %i of %lu. Value is: %f\n", count, recording.size(), output_.value);
          break;
          
        default:
          break;
      }
      
    }

  }
}

#endif


