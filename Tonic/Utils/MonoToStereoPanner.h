//
//  MonoToStereoPanner.h
//  Tonic 
//
//  Created by Morgan Packard on 2/28/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
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

#ifndef __Tonic__MonoToStereoPanner__
#define __Tonic__MonoToStereoPanner__

#include <iostream>
#include "Effect.h"
#include "ControlGenerator.h"

namespace Tonic {
  
  namespace Tonic_ {
  
    class MonoToStereoPanner_ : public Effect_{
      
    protected:
      ControlGenerator panControlGen;
      TonicFrames panFrames;
      
    public:
      MonoToStereoPanner_();
      ~MonoToStereoPanner_();
      void computeSynthesisBlock( const SynthesisContext_ &context );
      void setPan(ControlGenerator);
      
    };
    
    inline void MonoToStereoPanner_::computeSynthesisBlock(const SynthesisContext_ &context){
      input_.tick(synthesisBlock_, context);
      TonicFloat *synthBlockWriteHead = &synthesisBlock_[0];
      TonicFloat *synthBlockEnd = &synthesisBlock_[kSynthesisBlockSize * 2]; // is this safe?
      float panValue = panControlGen.tick(context).value;
      float leftVol = 1. - max(0., panValue);
      float rightVol = 1 + min(0., panValue);
      while(synthBlockWriteHead < synthBlockEnd){
          *(synthBlockWriteHead++) *= leftVol;
          *(synthBlockWriteHead++) *= rightVol;
      }
    }
    
  }
  
  class MonoToStereoPanner : public TemplatedEffect<MonoToStereoPanner, Tonic_::MonoToStereoPanner_>{
    
    public:
      // pan is a -1 to 1 value
      createControlGeneratorSetters(MonoToStereoPanner, pan, setPan);

  };
  
}

#endif /* defined(__Tonic__MonoToStereoPanner__) */


