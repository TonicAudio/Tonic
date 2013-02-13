//
//  Synth.cpp
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/23/13.
//
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

#include "Synth.h"
#include "SineWave.h"

namespace Tonic {

Synth::Synth(){
  
}

inline void Synth::tick( TonicFrames& frames ){
  
  // TODO -- add sequencers back in
  //        for (int i = 0; i < sequencers.size(); i++) {
  //          sequencers.at(i)->advanceBySamples(frames.frames());
  //        }
  outputGen.tick(frames);
  
  // TODO -- add message queue back in
  //        while (!messageQueue.empty()) {
  //            EZPlugSynthMessage msg = messageQueue.pop_front();
  //            switch (msg.messageType) {
  //                case SET_FIXED_VALUE:{
  //                    std::string target = msg.target;
  //                    pSetFixedValue(target, msg.value);
  //                    break;
  //                }
  //                case TRIGGER:{
  //                    std::string target = msg.target;
  //                    pTrigger(target);
  //                    break;
  //                }
  //            }
  //        }
  
  
  
};
  
}
