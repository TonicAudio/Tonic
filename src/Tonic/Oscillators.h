//
//  Oscillators.h
//  Tonic 
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__Oscillators__
#define __Tonic__Oscillators__

#include "Generator.h"
#include "SampleTable.h"

#define TONIC_OSC_RES 2048

namespace Tonic {
  
  namespace Tonic_ {
    
    //! Registry for all static lookup table data
    static TonicDictionary<SampleTable> s_oscTables_;
    
    //! Base fast table-lookup oscillator
    /*!
        TODO: Add phase sync/reset
     */
    class TableLookupOsc_ : public Generator_{
      
    protected:
      
      SampleTable lookupTable_;
      
      double phase_;
      
      Generator frequencyGenerator_;
      TonicFrames modFrames_;
      
      void computeSynthesisBlock( const SynthesisContext_ & context );
      
    public:
      
      TableLookupOsc_();

      //! Clear output and reset time pointer to zero.
      void reset( void );
      
      //! Set frequency generator input
      void setFrequency( Generator genArg){
        frequencyGenerator_ = genArg;
      }
      
      //! set sample table for lookup. MUST BE POWER OF 2 IN LENGTH
      void setLookupTable( SampleTable table );

    };
    
    inline void TableLookupOsc_::computeSynthesisBlock( const SynthesisContext_ & context ){
      
      // Update the frequency data
      frequencyGenerator_.tick(modFrames_, context);
      
      unsigned long tableSize = lookupTable_.size()-1;
      
      const TonicFloat rateConstant = (TonicFloat)tableSize / Tonic::sampleRate();
      
      TonicFloat *samples = &outputFrames_[0];
      TonicFloat *rateBuffer = &modFrames_[0];
      TonicFloat *tableData = lookupTable_.dataPointer();
      
      // R. Hoelderich style fast phasor.
      
      FastPhasor sd;
      
      // pre-multiply rate constant for speed
#ifdef USE_APPLE_ACCELERATE
      vDSP_vsmul(rateBuffer, 1, &rateConstant, rateBuffer, 1, kSynthesisBlockSize);
#else
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
        *rateBuffer++ *= rateConstant;
      }
      rateBuffer = &modFrames_[0];
#endif
      
      sd.d = BIT32DECPT;
      
      TonicInt32 offs;
      TonicInt32 msbi = sd.i[1];
      
      double frac;
      double ps = phase_ + BIT32DECPT;
      
      TonicFloat *tAddr, f1, f2;
      
      for ( unsigned int i=0; i<kSynthesisBlockSize; i++ ) {
        
        sd.d = ps;
        ps += *rateBuffer++;
        offs = sd.i[1] & (tableSize-1);
        tAddr = tableData + offs;
        sd.i[1] = msbi;
        frac = sd.d - BIT32DECPT;
        f1 = tAddr[0];
        f2 = tAddr[1];
        
        *samples++ = f1 + frac * (f2 - f1);
      }
      
      sd.d = BIT32DECPT * tableSize;
      msbi = sd.i[1];
      sd.d = ps + (BIT32DECPT * tableSize - BIT32DECPT);
      sd.i[1] = msbi;
      phase_ = sd.d - BIT32DECPT * tableSize;
      
    }


    // ---------------------------------------------------------------------------

    class RectWave_ : public Generator_
    {
      
    protected:
      
      Generator freqGen_;
      Generator pwmGen_;
      
      TonicFrames freqFrames_;
      TonicFrames pwmFrames_;
      
      double phaseAccum_;
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    public:
      
      RectWave_();
      
      void setFrequencyGenerator( Generator gen ){
        freqGen_ = gen;
      }
      
      void setPwmGenerator( Generator gen ){
        pwmGen_ = gen;
      }
      
      
    };
    
    inline void RectWave_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      // tick freq and pwm
      freqGen_.tick(freqFrames_, context);
      pwmGen_.tick(pwmFrames_, context);
      
      const TonicFloat rateConstant =  TONIC_OSC_RES / Tonic::sampleRate();
      
      TonicFloat *outptr = &outputFrames_[0];
      TonicFloat *freqptr = &freqFrames_[0];
      TonicFloat *pwmptr = &pwmFrames_[0];
      
      FastPhasor sd;
      
      // pre-multiply rate constant for speed
#ifdef USE_APPLE_ACCELERATE
      vDSP_vsmul(freqptr, 1, &rateConstant, freqptr, 1, kSynthesisBlockSize);
#else
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
        *freqptr++ *= rateConstant;
      }
      freqptr = &freqFrames_[0];
#endif
      
      sd.d = BIT32DECPT;
      TonicInt32 offs, msbi = sd.i[1];
      double ps = phaseAccum_ + BIT32DECPT;
      for ( unsigned int i=0; i<outputFrames_.frames(); i++ ) {
        
        sd.d = ps;
        ps += *freqptr++;
        offs = sd.i[1] & (TONIC_OSC_RES-1);
        sd.i[1] = msbi;
        
        *outptr++ = offs > (TONIC_OSC_RES * *pwmptr++) ? -1.0f : 1.0f;
      }
      
      sd.d = BIT32DECPT * TONIC_OSC_RES;
      msbi = sd.i[1];
      sd.d = ps + (BIT32DECPT * TONIC_OSC_RES - BIT32DECPT);
      sd.i[1] = msbi;
      phaseAccum_ = sd.d - BIT32DECPT * TONIC_OSC_RES;
      
    }

  // ---------------------------------------------------------------------------
  
    
    // Generator_ class to generate hard-edge rising saw, falling saw, triangle, or any angle in-between.
    
    class AngularWave_ : public Generator_{
      
    protected:
      
      Generator freqGen_;
      Generator slopeGen_;
      
      TonicFrames freqFrames_;
      TonicFrames slopeFrames_;
      
      double phaseAccum_;
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    public:
      
      AngularWave_();
      
      void setFrequencyGenerator( Generator gen ){
        freqGen_ = gen;
      }
      
      void setSlopeGenerator( Generator gen ){
        slopeGen_ = gen;
      }
      
      
    };
    
    inline void AngularWave_::computeSynthesisBlock(const SynthesisContext_ &context){
      
      // tick freq and slope inputs
      freqGen_.tick(freqFrames_, context);
      slopeGen_.tick(slopeFrames_, context);
      
      // calculate the output wave
      TonicFloat const rateConstant = TONIC_OSC_RES/Tonic::sampleRate();
      
      TonicFloat slope, frac, phase;
      TonicFloat *outptr = &outputFrames_[0];
      TonicFloat *freqptr = &freqFrames_[0];
      TonicFloat *slopeptr = &slopeFrames_[0];
      
      FastPhasor sd;
      
      // pre-multiply rate constant for speed
#ifdef USE_APPLE_ACCELERATE
      vDSP_vsmul(freqptr, 1, &rateConstant, freqptr, 1, kSynthesisBlockSize);
#else
      for (unsigned int i=0; i<kSynthesisBlockSize; i++){
        *freqptr++ *= rateConstant;
      }
      freqptr = &freqFrames_[0];
#endif
      
      sd.d = BIT32DECPT;
      TonicInt32 offs, msbi = sd.i[1];
      double ps = phaseAccum_ + BIT32DECPT;
      for ( unsigned int i=0; i<outputFrames_.frames(); i++ ) {
        
        // update the slope
        slope = clamp(*slopeptr++, 0.0f, 1.0f) * TONIC_OSC_RES;
        
        sd.d = ps;
        ps += *freqptr++;
        offs = sd.i[1] & (TONIC_OSC_RES-1);
        sd.i[1] = msbi;
        frac = sd.d - BIT32DECPT;
        
        phase = offs + frac;
        
        // TODO: this is still a bit slow. Maybe find a way to do it with a real table lookup
        
        // on the rising edge
        if (phase < slope){
          *outptr++ = (phase/slope)*2.0f - 1.0f;
        }
        // on the falling edge
        else{
          *outptr++ = (1.0f - ((phase - slope)/(TONIC_OSC_RES - slope))) * 2.0f - 1.0f;
        }
        
      }
      
      sd.d = BIT32DECPT * TONIC_OSC_RES;
      msbi = sd.i[1];
      sd.d = ps + (BIT32DECPT * TONIC_OSC_RES - BIT32DECPT);
      sd.i[1] = msbi;
      phaseAccum_ = sd.d - BIT32DECPT * TONIC_OSC_RES;
      
    }
    
  }
  
  // ---------------------------------------------------------------------------

  class TableLookupOsc : public TemplatedGenerator<Tonic_::TableLookupOsc_>{
    
  public:
    
    TableLookupOsc & setLookupTable( SampleTable lookupTable );
    
    createGeneratorSetters(TableLookupOsc, freq, setFrequency);
  };
  
  
  // ---------------------------------------------------------------------------

  //! Sine-wave oscillator
  class SineWave : public TemplatedGenerator<Tonic_::TableLookupOsc_>{
    
  public:
    
    SineWave();
    
    //! Frequency of the sine wave
    createGeneratorSetters(SineWave, freq, setFrequency)
    
  };
  
  
  // ---------------------------------------------------------------------------
  
  //! Quick-and-dirty rectangular wave.
  /*!
   WARNING: Is NOT anti-aliased!! Best for use as LFO, or if you just don't care :)
   */
  class RectWave : public TemplatedGenerator<Tonic_::RectWave_>{
    
  public:
    
    //! Set the frequency of the waveform
    createGeneratorSetters(RectWave, freq, setFrequencyGenerator);
    
    //! Set the pulse width of the rectangle. Input should be clipped between 0-1
    createGeneratorSetters(RectWave, pwm, setPwmGenerator);
    
  };
  
  // ---------------------------------------------------------------------------
  
  //! Quck-and-dirty square wave
  class SquareWave : public TemplatedGenerator<Tonic_::RectWave_>{
    
  public:
    
    SquareWave(){
      gen()->setPwmGenerator(FixedValue(0.5f));
    }
    
    createGeneratorSetters(SquareWave, freq, setFrequencyGenerator);
    
  };

  
  // ---------------------------------------------------------------------------
  
  //! Quick-and-dirty sawtooth oscillator
  /*!
   Not anti-aliased, so really best as an LFO. Can be used as an audio sawtooth oscillator in a pinch
   or if you don't mind some aliasing distortion.
   */
  class SawtoothWave : public TemplatedGenerator<Tonic_::AngularWave_>{
    
  public:
    
    createGeneratorSetters(SawtoothWave, freq, setFrequencyGenerator);
    
    //! set whether it's a descending sawtooth (default) or ascending
    SawtoothWave & isAscending(bool ascending){
      gen()->setSlopeGenerator(FixedValue(ascending ? 1.f : 0.f));
      return *this;
    }
  };
  
  // ---------------------------------------------------------------------------

  //! Quick-and-dirty triangle wave.
  class TriangleWave : public TemplatedGenerator<Tonic_::AngularWave_>{
    
  public:
    
    TriangleWave(){
      gen()->setSlopeGenerator(FixedValue(0.5f));
    }
    
    createGeneratorSetters(TriangleWave, freq, setFrequencyGenerator);
    
    //! Set from 0-1 to change slope. At 0, it's a falling saw, at 1, it's a rising saw (defaults to 0.5, triangle)
    createGeneratorSetters(TriangleWave, slope, setSlopeGenerator);
    
  };
  

}

#endif /* defined(__Tonic__Oscillators__) */


