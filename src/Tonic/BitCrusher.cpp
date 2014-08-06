//
//  BitCrusher.cpp
//
//  Created by Morgan Packard on 7/17/14.
//
//

#include "BitCrusher.h"

namespace Tonic{

  namespace Tonic_{
  
      BitCrusher_::BitCrusher_(){
        setBitDepth(ControlValue(16));
      }
    
      void BitCrusher_::setInput( Generator input ) {
        input_ = input;
        setIsStereoInput(input_.isStereoOutput());
      };
    
      void BitCrusher_::setIsStereoInput( bool stereo ) {
        if (stereo != isStereoInput_){
          dryFrames_.resize(kSynthesisBlockSize, stereo ? 2 : 1, 0);
          outputFrames_.resize(kSynthesisBlockSize, stereo ? 2 : 1, 0);
        }
        isStereoInput_ = stereo;
        isStereoOutput_ = stereo;
      };
    
      void BitCrusher_::setBitDepth(ControlGenerator bitDepthArg){
        bitDepth = bitDepthArg;
      }
    
  }

}

