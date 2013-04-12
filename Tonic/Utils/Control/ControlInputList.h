//
//  ControlInputList.h
//  Tonic 
//
//  Created by Morgan Packard on 4/11/13.
//
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ControlInputList__
#define __Tonic__ControlInputList__

#include "ControlGenerator.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlInputList_ : public ControlGenerator_{
      
    protected:
      void computeOutput(const SynthesisContext_ & context);
      vector<ControlGenerator>inputs;
      ControlGenerator inputIndex;
      
    public:
      ControlInputList_();
      ~ControlInputList_();
      void addInput(ControlGenerator input);
      void setInputIndex(ControlGenerator inputIndexArg);
      
    };
    
  }
  
  class ControlInputList : public TemplatedControlGenerator<Tonic_::ControlInputList_>{
    
  public:
    createControlGeneratorSetters(ControlInputList, addInput, addInput);
    createControlGeneratorSetters(ControlInputList, inputIndex, setInputIndex);

  };
}

#endif /* defined(__Tonic__ControlInputList__) */


