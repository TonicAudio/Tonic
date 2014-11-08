//
//  Filters.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/22/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_FILTERS_H
#define TONIC_FILTERS_H

#include "Effect.h"
#include "FilterUtils.h"
#include "Generator.h"
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
      
      bool bNormalizeGain_;
      
      void computeSynthesisBlock( const SynthesisContext_ & context );
      
      // subclasses override to compute new coefficients and apply filter
      virtual void applyFilter( TonicFloat cutoff, TonicFloat Q,  const SynthesisContext_ & context ) = 0;
      
    public:
      
      Filter_();
      
      // Overridden so output channel layout follows input channel layout
      virtual void setInput( Generator input );
      
      void setNormalizesGain( bool norm ) { bNormalizeGain_ = norm; };
      void setCutoff( Generator cutoff ){ cutoff_ = cutoff; };
      void setQ( Generator Q ){ Q_ = Q; }
      void setBypass( ControlGenerator bypass ){ bypass_ = bypass; };

            
    };
    
    inline void Filter_::computeSynthesisBlock( const SynthesisContext_ & context ){
      
      TonicFloat cCutoff;
      TonicFloat cQ;
      
      // get cutoff and Q inputs
      // For now only using first frame of output. Setting coefficients each frame is very inefficient.
      // Updating cutoff every 64-samples is typically fast enough to avoid audible artifacts when sweeping filters.
      
      cutoff_.tick(workspace_, context);
      cCutoff = clamp(workspace_(0,0), 20, sampleRate()/2); // clamp to reasonable range
      
      Q_.tick(workspace_, context);
      cQ = max(workspace_(0,0), 0.7071); // clamp to reasonable range
      
      applyFilter(cCutoff, cQ, context);
      
    }
    
    // ===============================
    //            LPF 6
    // ===============================
    
    //! One-pole lowpass filter. Q is undefined for this filter
    class LPF6_ : public Filter_ {
      
    private:
      
      TonicFloat lastOut_[2];
      
    protected:
      inline void applyFilter( TonicFloat cutoff, TonicFloat Q, const SynthesisContext_ & context )
      {
        
        TonicFloat *inptr = &dryFrames_[0];
        TonicFloat *outptr = &outputFrames_[0];
        TonicFloat coef = cutoffToOnePoleCoef(cutoff);
        TonicFloat norm = bNormalizeGain_ ? 1.0f - coef : 1.0f;
        unsigned int nChannels = dryFrames_.channels();
        
        for (unsigned int i=0; i<kSynthesisBlockSize; i++){
          for (unsigned int c=0; c<nChannels; c++){
            lastOut_[c] = (norm * (*inptr++)) + (coef * lastOut_[c]);
            *outptr++ = lastOut_[c];
          }
        }
      }
      
    public:
      
      LPF6_(){
        lastOut_[0] = 0;
        lastOut_[1] = 0;
      }
      
    };
    
    // ===============================
    //            HPF 6
    // ===============================
    
    //! One-pole highpass filter. Q is undefined for this filter
    class HPF6_ : public Filter_ {
      
    private:
      
      TonicFloat lastOut_[2];
      
    protected:
      
      inline void applyFilter( TonicFloat cutoff, TonicFloat Q, const SynthesisContext_ & context )
      {
        
        TonicFloat *inptr = &dryFrames_[0];
        TonicFloat *outptr = &outputFrames_[0];
        TonicFloat coef = 1.0f - cutoffToOnePoleCoef(cutoff);
        TonicFloat norm = bNormalizeGain_ ? 1.0f - coef : 1.0f;
        unsigned int nChannels = dryFrames_.channels();
        
        for (unsigned int i=0; i<kSynthesisBlockSize; i++){
          for (unsigned int c=0; c<nChannels; c++){
            lastOut_[c] = (norm * (*inptr++)) - (coef * lastOut_[c]);
            *outptr++ = lastOut_[c];
          }
        }

      }
      
    public:
      
      HPF6_(){
        lastOut_[0] = 0;
        lastOut_[1] = 0;
      }

    };
    
    
    // ===============================
    //            LPF 12
    // ===============================
      
    //! Butterworth 2-pole LPF
    class LPF12_ : public Filter_ {

    private:
      
      Biquad biquad_;
      
    protected:
      
      inline void applyFilter( TonicFloat cutoff, TonicFloat Q, const SynthesisContext_ & context ){
        // set coefficients
        TonicFloat newCoef[5];
        bltCoef(0, 0, bNormalizeGain_ ? 1.0f/Q : 1.0f, 1.0f/Q, 1, cutoff, newCoef);
        biquad_.setCoefficients(newCoef);
        
        // compute
        biquad_.filter(dryFrames_, outputFrames_);
      }
      
    public:
      
      void setIsStereoInput( bool isStereoInput )
      {
        Filter_::setIsStereoInput(isStereoInput);
        biquad_.setIsStereo(isStereoInput);
      }

    };
    
    // ===============================
    //            LPF 24
    // ===============================
    
    //! Butterworth 4-pole LPF
    class LPF24_ : public Filter_ {
      
    private:
      
      Biquad biquads_[2];
      
    protected:
      
      inline void applyFilter( TonicFloat cutoff, TonicFloat Q, const SynthesisContext_ & context ){
        // set coefficients
        TonicFloat newCoef[5];
        
        // stage 1
        bltCoef(0, 0, bNormalizeGain_ ? 1.0f/Q : 1.0f, 0.5412f/Q, 1, cutoff, newCoef);
        biquads_[0].setCoefficients(newCoef);
        
        // stage 2
        bltCoef(0, 0, bNormalizeGain_ ? 1.0f/Q : 1.0f, 1.3066f/Q, 1, cutoff, newCoef);
        biquads_[1].setCoefficients(newCoef);
        
        // compute
        biquads_[0].filter(dryFrames_, outputFrames_);
        biquads_[1].filter(outputFrames_, outputFrames_);
      }
      
    public:
      
      void setIsStereoInput( bool isStereoInput )
      {
        Filter_::setIsStereoInput(isStereoInput);
        biquads_[0].setIsStereo(isStereoInput);
        biquads_[1].setIsStereo(isStereoInput);
      }

      
    };
    
    // ===============================
    //            HPF 12
    // ===============================
    
    //! Butterworth 2-pole HPF
    class HPF12_ : public Filter_ {
      
    private:
      
      Biquad biquad_;
   
    protected:
      
      inline void applyFilter( TonicFloat cutoff, TonicFloat Q, const SynthesisContext_ & context ){
        
        // set coefficients
        TonicFloat newCoef[5];
        bltCoef(bNormalizeGain_ ? 1.0f/Q : 1.0f, 0, 0, 1.0f/Q, 1, cutoff, newCoef);
        biquad_.setCoefficients(newCoef);
        
        // compute
        biquad_.filter(dryFrames_, outputFrames_);
      }
      
    public:
      
      void setIsStereoInput( bool isStereoInput )
      {
        Filter_::setIsStereoInput(isStereoInput);
        biquad_.setIsStereo(isStereoInput);
      }
      
    };
    
    // ===============================
    //            HPF 24
    // ===============================
    
    //! Butterworth 4-pole HPF
    class HPF24_ : public Filter_ {
      
    private:
      
      Biquad biquads_[2];
      
    protected:
      
      inline void applyFilter( TonicFloat cutoff, TonicFloat Q, const SynthesisContext_ & context ){
        // set coefficients
        TonicFloat newCoef[5];
        
        // stage 1
        bltCoef(bNormalizeGain_ ? 1.0f/Q : 1.0f, 0, 0, 0.5412f/Q, 1, cutoff, newCoef);
        biquads_[0].setCoefficients(newCoef);
        
        // stage 2
        bltCoef(bNormalizeGain_ ? 1.0f/Q : 1.0f, 0, 0, 1.3066f/Q, 1, cutoff, newCoef);
        biquads_[1].setCoefficients(newCoef);
        
        // compute
        biquads_[0].filter(dryFrames_, outputFrames_);
        biquads_[1].filter(outputFrames_, outputFrames_);
      }
      
    public:
      
      void setIsStereoInput( bool isStereoInput )
      {
        Filter_::setIsStereoInput(isStereoInput);
        biquads_[0].setIsStereo(isStereoInput);
        biquads_[1].setIsStereo(isStereoInput);
      }
      
    };
    
    // ===============================
    //            BPF 12
    // ===============================
    
    //! Butterworth 2-pole BPF, constant 0dB peak
    class BPF12_ : public Filter_ {
      
    private:
      
      Biquad biquad_;
      
    protected:
      
      
      inline void applyFilter( TonicFloat cutoff, TonicFloat Q, const SynthesisContext_ & context ){
        
        // set coefficients
        TonicFloat newCoef[5];
        bltCoef(0, bNormalizeGain_ ? 1.0f/Q : 1.0f, 0, 1.0f/Q, 1, cutoff, newCoef);
        biquad_.setCoefficients(newCoef);
        
        // compute
        biquad_.filter(dryFrames_, outputFrames_);
      }
      
    public:
      
      virtual void setIsStereoInput( bool isStereoInput )
      {
        Filter_::setIsStereoInput(isStereoInput);
        biquad_.setIsStereo(isStereoInput);
      }
      
    };
    
    // ===============================
    //            BPF 24
    // ===============================
    
    //! Butterworth 4-pole BPF
    class BPF24_ : public Filter_ {
      
    private:
      
      Biquad biquads_[2];
      
    protected:
      
      inline void applyFilter( TonicFloat cutoff, TonicFloat Q, const SynthesisContext_ & context ){
        // set coefficients
        TonicFloat newCoef[5];
        
        // stage 1
        bltCoef(0, bNormalizeGain_ ? 1.0f/Q : 1.0f, 0, 0.5412f/Q, 1, cutoff, newCoef);
        biquads_[0].setCoefficients(newCoef);
        
        // stage 2
        bltCoef(0, bNormalizeGain_ ? 1.0f/Q : 1.0f, 0, 1.3066f/Q, 1, cutoff, newCoef);
        biquads_[1].setCoefficients(newCoef);
        
        // compute
        biquads_[0].filter(dryFrames_, outputFrames_);
        biquads_[1].filter(outputFrames_, outputFrames_);
      }
      
    public:
      
      void setIsStereoInput( bool isStereoInput )
      {
        Filter_::setIsStereoInput(isStereoInput);
        biquads_[0].setIsStereo(isStereoInput);
        biquads_[1].setIsStereo(isStereoInput);
      }
      
    };
  }
  
#pragma mark - Smart Pointers
  
  template<class FilterType, class FilterType_>
  class TemplatedFilter : public TemplatedEffect<FilterType, FilterType_>{
  
  public:
    
    TONIC_MAKE_GEN_SETTERS(FilterType, cutoff, setCutoff);
    TONIC_MAKE_GEN_SETTERS(FilterType, Q, setQ);
    FilterType & normalizesGain(bool norm){
      this->gen()->setNormalizesGain(norm);
      return static_cast<FilterType&>(*this);
    }

  };
  

  // ------------------- Smart Pointers -----------------------
  
  // LPF 6
  class LPF6 : public TemplatedFilter<LPF6, Tonic_::LPF6_>{};
  
  // HPF 6
  class HPF6 : public TemplatedFilter<HPF6, Tonic_::HPF6_>{};

  // LPF 12
  class LPF12 : public TemplatedFilter<LPF12, Tonic_::LPF12_>{};
  
  // LPF 24
  class LPF24 : public TemplatedFilter<LPF24, Tonic_::LPF24_>{};
  
  // HPF 12
  class HPF12 : public TemplatedFilter<HPF12, Tonic_::HPF12_>{};
  
  // HPF 24
  class HPF24 : public TemplatedFilter<HPF24, Tonic_::HPF24_>{};
  
  // BPF 12
  class BPF12 : public TemplatedFilter<BPF12, Tonic_::BPF12_>{};
  
  // BPF 24
  class BPF24 : public TemplatedFilter<BPF24, Tonic_::BPF24_>{};
  
  
}

#endif


