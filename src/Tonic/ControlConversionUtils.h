//
//  ControlConversionUtils.h
//  TonicDemo
//


#ifndef __TonicDemo__ControlConversionUtils__
#define __TonicDemo__ControlConversionUtils__

#include "ControlConditioner.h"

namespace Tonic{

  namespace Tonic_{

    class ControlFloor_ : public ControlConditioner_{
      
    protected:
      inline void computeOutput(const SynthesisContext_ & context){
        output_.value = (int)input_.tick(context).value;
        output_.triggered = input_.tick(context).triggered;
      }
      
    };
    
    //-------------------------------------------------

    class ControlRound_ : public ControlConditioner_{
      
    protected:
      inline void computeOutput(const SynthesisContext_ & context){
        output_.value = roundf(input_.tick(context).value);
        output_.triggered = input_.tick(context).triggered;
      }
      
    };
    
    //-------------------------------------------------

    class ControlMidiToFreq_ : public ControlConditioner_{
     
    protected:
      
      inline void computeOutput(const SynthesisContext_ & context){
        
        ControlGeneratorOutput inputOut = input_.tick(context);
        output_.triggered = inputOut.triggered;
        output_.value = mtof( inputOut.value );
        
      }
      
    };
    
    //-------------------------------------------------
    
    class ControlDbToLinear_ : public ControlConditioner_{
      
    protected:
      
      inline void computeOutput(const SynthesisContext_ & context){
      
        ControlGeneratorOutput inputOutput = input_.tick(context);
        output_.triggered = inputOutput.triggered;
        if (inputOutput.triggered){
          output_.value = dBToLin(inputOutput.value);
        }
        
      }
      
    };
    
  }
  
  //! Floors incoming float value to integer
  class ControlFloor : public TemplatedControlConditioner<ControlFloor, Tonic_::ControlFloor_> {};
  
  //-------------------------------------------------
  
  //! Rounds (towards zero) incoming float value to integer
  class ControlRound : public TemplatedControlConditioner<ControlFloor, Tonic_::ControlRound_> {};
  
  //-------------------------------------------------

  //! Converts midi note number to frequency in Hz
  class ControlMidiToFreq : public TemplatedControlConditioner<ControlMidiToFreq, Tonic_::ControlMidiToFreq_> {};

  //-------------------------------------------------
  
  //! Converts dbFS values (0 dB = 1.0) to a linear gain/attenuation value
  class ControlDbToLinear : public TemplatedControlConditioner<ControlDbToLinear, Tonic_::ControlDbToLinear_> {};
  
  //-------------------------------------------------

  
}


#endif /* defined(__TonicDemo__ControlConversionUtils__) */
