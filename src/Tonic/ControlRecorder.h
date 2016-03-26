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
  
  //! Record and play back a control-rate data stream
  class ControlRecorder :  public TemplatedControlConditioner<ControlRecorder, Tonic_::ControlRecorder_>{
    
  public:
    
    enum Mode{
      RECORD,
      PLAY,
      STOP
    };
    
    TONIC_MAKE_CTRL_GEN_SETTERS(ControlRecorder, mode, setMode)

  };
  
  //-- put down here so we can use the enum
  namespace Tonic_ {
    
    inline void ControlRecorder_::computeOutput(const SynthesisContext_ & context){
      
      ControlGeneratorOutput inputOut = input_.tick(context);
      ControlGeneratorOutput modeOut = mode.tick(context);
     
      ControlRecorder::Mode newMode = (ControlRecorder::Mode)((int)modeOut.value);

      //-- If there's no recording, PLAY mode behaves the same as STOP mode
      if (newMode == ControlRecorder::PLAY && recording.empty())
      {
        newMode = ControlRecorder::STOP;
      }
    
      
      if(newMode!= currentMode){
        currentMode = newMode;
        if(newMode == ControlRecorder::STOP){
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
          break;
          
        default:
          break;
      }
      
    }

  }
}

#endif


