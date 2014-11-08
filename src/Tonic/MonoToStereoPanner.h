//
//  MonoToStereoPanner.h
//  Tonic 
//
//  Created by Morgan Packard on 2/28/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_MONOTOSTEREOPANNER_H
#define TONIC_MONOTOSTEREOPANNER_H

#include <iostream>
#include "Effect.h"
#include "ControlGenerator.h"

namespace Tonic {
  
  namespace Tonic_ {
  
    class MonoToStereoPanner_ : public Effect_{
      
    protected:
      ControlGenerator panControlGen;
      TonicFrames panFrames;
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    public:
      MonoToStereoPanner_();
      ~MonoToStereoPanner_();
      void setPan(ControlGenerator);
      
    };
    
    inline void MonoToStereoPanner_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      TonicFloat *synthBlockWriteHead = &outputFrames_[0];
      TonicFloat *dryFramesReadHead = &dryFrames_[0];
      
      unsigned int nSamples = kSynthesisBlockSize;
      float panValue = panControlGen.tick(context).value;
      float leftVol = 1. - max(0., panValue);
      float rightVol = 1 + min(0., panValue);
      while (nSamples--){
          *(synthBlockWriteHead++) = *dryFramesReadHead * leftVol;
          *(synthBlockWriteHead++) = *dryFramesReadHead++ * rightVol;
      }
    }
    
  }
  
  class MonoToStereoPanner : public TemplatedEffect<MonoToStereoPanner, Tonic_::MonoToStereoPanner_>{
    
    public:
      // pan is a -1 to 1 value
      TONIC_MAKE_CTRL_GEN_SETTERS(MonoToStereoPanner, pan, setPan);

  };
  
}

#endif


