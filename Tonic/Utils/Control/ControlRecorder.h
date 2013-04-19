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
}

#endif /* defined(__Tonic__ControlRecorder__) */


