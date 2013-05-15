//
//  ControlMetro.h
//  Tonic 
//
//  Created by Nick Donaldson on 3/10/13.

//
// See LICENSE.txt for license and usage information.
//


// Metronome! Ouputs a "changed" status at a regular BPM interval

#ifndef __Tonic__ControlMetro__
#define __Tonic__ControlMetro__

#include "ControlGenerator.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlMetro_ : public ControlGenerator_{
      
    protected:
      
      double lastClickTime_;
      
      ControlGenerator bpm_;
      
      void computeOutput(const SynthesisContext_ & context);
          
    public:

      ControlMetro_();
      
      void setBPMGen( ControlGenerator bpmGen ){ bpm_ = bpmGen; };
      
    };
    
  }
  
  class ControlMetro : public TemplatedControlGenerator<Tonic_::ControlMetro_>{
    
  public:
    
    ControlMetro(float bpm = 120){
      gen()->setBPMGen(ControlValue(bpm));
    }
    
    createControlGeneratorSetters(ControlMetro, bpm, setBPMGen);
  };
}

#endif /* defined(__Tonic__ControlMetro__) */


