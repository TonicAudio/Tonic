//
//  ControlMetro.h
//  Tonic 
//
//  Created by Nick Donaldson on 3/10/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
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
    
      void computeOutput(const SynthesisContext_ & context);
    
      ControlGenerator bpm_;
      double lastClickTime_;
      
    public:
      ControlMetro_();
      ~ControlMetro_();
      
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


