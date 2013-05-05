//
//  Reverb.h
//  Tonic 
//
//  Created by Nick Donaldson on 5/4/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
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

#ifndef __Tonic__Reverb__
#define __Tonic__Reverb__

#include "Effect.h"
#include "DelayUtils.h"
#include "CombFilter.h"

// Number of FF comb filters
#define  TONIC_REVERB_N_COMBS 8

namespace Tonic {
  
  namespace Tonic_ {
    
    //! Moorer-Schroeder style Artificial Reverb effect
    /*!
        - [ ] Pre-delay
        - [ ] Input filter
        - [ ] Early reflection taps
        - [ ] Decay time and decay filtering
        - [ ] Variable "Room size"
        - [ ] Variable stereo width
     */
    
    class Reverb_ : public Effect_
    {
      protected:
      
        // Filters and delay lines
        DelayLine     preDelayLine_;
        DelayLine     reflectDelayLine_;
      
        ControlGenerator preDelayTimeCtrlGen_;
        ControlGenerator roomSizeCtrlGen_;
        ControlGenerator densityCtrlGen_; // affects number of early reflection taps
      
      
      public:
      
        Reverb_();
        ~Reverb_();
    
        // Overridden so output channel layout follows input channel layout
        void setInput( Generator input );
        
        void computeSynthesisBlock( const SynthesisContext_ &context );
      
    };
    
    inline void Reverb_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      
      
    }
        
  }
  
  class Reverb : public TemplatedEffect<Reverb, Tonic_::Reverb_>
  {

    public:
    
      Reverb();
      
    
  };
}

#endif /* defined(__Tonic__Reverb__) */


