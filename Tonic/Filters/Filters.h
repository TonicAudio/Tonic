//
//  Filters.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/22/13.
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

#ifndef __Tonic__Filters__
#define __Tonic__Filters__

#include <iostream>
#include "Effect.h"
#include "Biquad.h"
#include "FilterUtils.h"
#include "FixedValue.h"
#include "ControlGenerator.h"

/**********************************************
 * 
 *  IIR filter modules
 *
 *  WARNING - Use of high Q values may
 *  cause clipping. It's recommended that you
 *  add a limiter or greatly reduce the gain
 *  after a filter stage.
 *
 **********************************************/

namespace Tonic {
  
#pragma mark - Core Generators
  
  // ------------------- Core Generators -----------------------
  
  namespace Tonic_ {

    // ================================
    //       Filter Base Class
    // ================================
    
    //! Basic filter Effect_ subclass with inputs for cutoff and Q
    class Filter_ : public Effect_{
        
    protected:
      
      TonicFrames workspace_;
      
      Generator cutoff_;
      Generator Q_;
      ControlGenerator bypass_;
      
    public:
      
      Filter_();
      
      inline void setCutoff( Generator cutoff ){ cutoff_ = cutoff; };
      inline void setQ( Generator Q ){ Q_ = Q; }
      inline void setBypass( ControlGenerator bypass ){ bypass_ = bypass; };
      
      // subclasses override to compute new coefficients and apply filter to passed-in frames
      virtual void applyFilter( TonicFloat cutoff, TonicFloat Q, TonicFrames & frames ) = 0;
      
      inline void tick( TonicFrames& frames){
          
        TonicFloat cCutoff;
        TonicFloat cQ;
        
        lockMutex();
        
        // get cutoff and Q inputs
        // For now only using first frame of output. Setting coefficients each frame is very inefficient.
        // Updates every 64-samples is typically fast enough to avoid artifacts when sweeping filters.
        
        cutoff_.tick(workspace_);
        cCutoff = clamp(workspace_(0,0), 20, sampleRate()/2);
        Q_.tick(workspace_);
        cQ = max(workspace_(0,0), 0);
        
        // get input frames
        input_.tick(frames);
        
        unlockMutex();
        
        applyFilter(cCutoff, cQ, frames);
      }
      
    };
    
    // ===============================
    //            LPF 12
    // ===============================
      
    //! Butterworth 2-pole LPF
    class LPF12_ : public Filter_ {

    protected:
      
      Biquad_ biquad_;
      
    public:
      
      inline void applyFilter( TonicFloat cutoff, TonicFloat Q, TonicFrames & frames ){
          // set coefficients
          TonicFloat newCoef[5];
          bltCoef(0, 0, 1.0f/Q, 1.0f/Q, 1, cutoff, newCoef);
          biquad_.setCoefficients(newCoef);

          // compute
          biquad_.tick(frames);
      }
      
    };
    
    // ===============================
    //            LPF 24
    // ===============================
    
    //! Butterworth 4-pole LPF
    class LPF24_ : public Filter_ {
      
    protected:
      
      Biquad_ biquad1_;
      Biquad_ biquad2_;
      
    public:
      
      inline void applyFilter( TonicFloat cutoff, TonicFloat Q, TonicFrames & frames ){
        // set coefficients
        TonicFloat newCoef[5];
        
        // stage 1
        bltCoef(0, 0, 1.0f/Q, 0.5412f/Q, 1, cutoff, newCoef);
        biquad1_.setCoefficients(newCoef);
        
        // stage 2
        bltCoef(0, 0, 1.0f/Q, 1.3066f/Q, 1, cutoff, newCoef);
        biquad2_.setCoefficients(newCoef);
        
        // compute
        biquad1_.tick(frames);
        biquad2_.tick(frames);
      }
      
    };
    
    // ===============================
    //            HPF 12
    // ===============================
    
    //! Butterworth 2-pole HPF
    class HPF12_ : public Filter_ {
      
    protected:
      
      Biquad_ biquad_;
      
    public:
      
      inline void applyFilter( TonicFloat cutoff, TonicFloat Q, TonicFrames & frames ){
        
        // set coefficients
        TonicFloat newCoef[5];
        bltCoef(1.0f/Q, 0, 0, 1.0f/Q, 1, cutoff, newCoef);
        biquad_.setCoefficients(newCoef);
        
        // compute
        biquad_.tick(frames);
      }
      
    };
    
    // ===============================
    //            HPF 24
    // ===============================
    
    //! Butterworth 4-pole HPF
    class HPF24_ : public Filter_ {
      
    protected:
      
      Biquad_ biquad1_;
      Biquad_ biquad2_;
      
    public:
      
      inline void applyFilter( TonicFloat cutoff, TonicFloat Q, TonicFrames & frames ){
        // set coefficients
        TonicFloat newCoef[5];
        
        // stage 1
        bltCoef(1.0f/Q, 0, 0, 0.5412f/Q, 1, cutoff, newCoef);
        biquad1_.setCoefficients(newCoef);
        
        // stage 2
        bltCoef(1.0f/Q, 0, 0, 1.3066f/Q, 1, cutoff, newCoef);
        biquad2_.setCoefficients(newCoef);
        
        // compute
        biquad1_.tick(frames);
        biquad2_.tick(frames);
      }
      
    };
    
    // ===============================
    //            BPF 12
    // ===============================
    
    //! Butterworth 2-pole BPF, constant 0dB peak
    class BPF12_ : public Filter_ {
      
    protected:
      
      Biquad_ biquad_;
      
    public:
      
      inline void applyFilter( TonicFloat cutoff, TonicFloat Q, TonicFrames & frames ){
        
        // set coefficients
        TonicFloat newCoef[5];
        bltCoef(0, 1.0f/Q, 0, 1.0f/Q, 1, cutoff, newCoef);
        biquad_.setCoefficients(newCoef);
        
        // compute
        biquad_.tick(frames);
      }
      
    };
    
    // ===============================
    //            BPF 24
    // ===============================
    
    //! Butterworth 4-pole BPF
    class BPF24_ : public Filter_ {
      
    protected:
      
      Biquad_ biquad1_;
      Biquad_ biquad2_;
      
    public:
      
      inline void applyFilter( TonicFloat cutoff, TonicFloat Q, TonicFrames & frames ){
        // set coefficients
        TonicFloat newCoef[5];
        
        // stage 1
        bltCoef(0, 1.0f/Q, 0, 0.5412f/Q, 1, cutoff, newCoef);
        biquad1_.setCoefficients(newCoef);
        
        // stage 2
        bltCoef(0, 1.0f/Q, 0, 1.3066f/Q, 1, cutoff, newCoef);
        biquad2_.setCoefficients(newCoef);
        
        // compute
        biquad1_.tick(frames);
        biquad2_.tick(frames);
      }
      
    };
  }
  
#pragma mark - Smart Pointers
  
  // macro to create smart pointer for a given filter subtype, since many will be identical
#define createFilterSubtype(filterClassName)                                        \
                                                                                    \
                                                                                    \
  class filterClassName : public TemplatedGenerator<Tonic_::filterClassName ## _>{  \
                                                                                    \
  public:                                                                           \
                                                                                    \
    createGeneratorSetters(filterClassName, input, setInput);                       \
    createGeneratorSetters(filterClassName, cutoff, setCutoff);                     \
    createGeneratorSetters(filterClassName, Q, setQ);                               \
  }                                                                                 \

  // ------------------- Smart Pointers -----------------------
  

  // LPF 12
  createFilterSubtype(LPF12);
  
  // LPF 24
  createFilterSubtype(LPF24);
  
  // HPF 12
  createFilterSubtype(HPF12);
  
  // HPF 24
  createFilterSubtype(HPF24);
  
  // BPF 12
  createFilterSubtype(BPF12);
  
  // BPF 24
  createFilterSubtype(BPF24);
  
  
}

#endif /* defined(__Tonic__Filters__) */

