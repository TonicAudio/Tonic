//
//  ControlGenerator.h
//  TonicDemo
//
//  Created by Morgan Packard on 2/12/13.

//


#ifndef __Tonic_ControlGenerator__
#define __Tonic_ControlGenerator__

#include "TonicCore.h"

namespace Tonic {
  
  struct ControlGeneratorOutput{
    
    TonicFloat  value;
    bool        triggered;
    
    ControlGeneratorOutput() : value(0), triggered(false) {};
  };

  namespace Tonic_{

    class ControlGenerator_{
      
    public:
    
      ControlGenerator_();
      virtual ~ControlGenerator_();
            
      // mutex for swapping inputs, etc
      void lockMutex();
      void unlockMutex();
      
      // Only override tick if you need custom reuse behavior
      // Pass in a pointer to a TonicFloat to return a value. Some generators may not care about value.
      virtual ControlGeneratorOutput tick( const SynthesisContext_ & context );
      
      // Used for initializing other generators (see smoothed() method for example)
      virtual ControlGeneratorOutput initialOutput();
      
    protected:
      
      //! Override this function to implement a new ControlGenerator
      /*!
          Subclasses should use this function to put new data into output_
      */
      virtual void computeOutput(const SynthesisContext_ & context) {};
      
      ControlGeneratorOutput  output_;
      unsigned long           lastFrameIndex_;
      
    };
    
    inline ControlGeneratorOutput ControlGenerator_::tick(const SynthesisContext_ & context){
      
      if (context.forceNewOutput || lastFrameIndex_ != context.elapsedFrames){
        lastFrameIndex_ = context.elapsedFrames;
        computeOutput(context);
      }
      
#ifdef TONIC_DEBUG
      if(output_.value != output_.value){
        Tonic::error("ControlGenerator_::tick NaN detected.", true);
      }
#endif
      
      return output_;
    }

  }

  // forward declaration
  class RampedValue;

  class ControlGenerator : public TonicSmartPointer<Tonic_::ControlGenerator_>{

  public:
    
    ControlGenerator(Tonic_::ControlGenerator_ * cGen = NULL) : TonicSmartPointer<Tonic_::ControlGenerator_>(cGen) {}
    
    inline ControlGeneratorOutput tick( const Tonic_::SynthesisContext_ & context ){
      return obj->tick(context);
    }
    
    // shortcut for creating ramped value
    RampedValue smoothed(float length = 0.05);
    
  };

  
  template<class GenType>
  class TemplatedControlGenerator : public ControlGenerator{
  protected:
    GenType* gen(){
      return static_cast<GenType*>(obj);
    }
    
  public:
    TemplatedControlGenerator() : ControlGenerator(new GenType) {}
    
  };

  
}

#include "ControlValue.h"

#define createControlGeneratorSetters(generatorClassName, methodNameInGenerator, methodNameInGenerator_)\
\
generatorClassName& methodNameInGenerator(float arg){                              \
return methodNameInGenerator( ControlValue(arg) );                                 \
}                                                                                  \
\
generatorClassName& methodNameInGenerator(ControlGenerator arg){                   \
this->gen()->methodNameInGenerator_(arg);                                          \
return static_cast<generatorClassName&>(*this);                                    \
}


#endif
