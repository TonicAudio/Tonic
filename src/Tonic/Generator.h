//
//  Generator.h
//  Tonic
//
//  Created by Morgan Packard on 1/23/13.
//
//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_GENERATOR_H
#define TONIC_GENERATOR_H

#include "TonicFrames.h"
#include <cmath>
namespace Tonic {

  namespace Tonic_{

    class Generator_{
      
    public:
      
      Generator_();
      virtual ~Generator_();

    // Keep track of whether this Generator_ has been added, via TONIC_MAKE_GEN_SETTERS to another generator
    // If it has been added, the number of channels should not be allowed to change
    bool hasBeenAdded; 
      
      virtual void tick( TonicFrames& frames, const SynthesisContext_ &context );
      
      bool isStereoOutput(){ return isStereoOutput_; };
      
      // set stereo/mono - changes number of channels in outputFrames_
      // subclasses should call in constructor to determine channel output
      virtual void setIsStereoOutput( bool stereo );
      
    protected:
      
      // override point for defining generator behavior
      // subclasses should implment to fill frames with new data
      virtual void computeSynthesisBlock( const SynthesisContext_ &context ) {};

      
      bool            isStereoOutput_;
      TonicFrames     outputFrames_;
      unsigned long   lastFrameIndex_;
      
    };
    
    inline void Generator_::tick(TonicFrames &frames, const SynthesisContext_ &context ){
      
      // check context to see if we need new frames
      if (context.forceNewOutput || lastFrameIndex_ != context.elapsedFrames){
        computeSynthesisBlock(context);
        lastFrameIndex_ = context.elapsedFrames;
      }
    
      // copy synthesis block to frames passed in
      frames.copy(outputFrames_);
      
    }

  }

  //-- forward delcaration
  class MonoToStereoPanner;
  class ControlGenerator;
  
  class Generator : public TonicSmartPointer<Tonic_::Generator_>{

  public:
    
    Generator( Tonic_::Generator_ * gen = new Tonic_::Generator_ ) : TonicSmartPointer<Tonic_::Generator_>(gen) {}
    
    inline bool isStereoOutput(){
      return obj->isStereoOutput();
    }
    
    virtual void tick(TonicFrames& frames, const Tonic_::SynthesisContext_ & context){
      obj->tick(frames, context);
    }

    //! Shortcut for wrapping with a Panner. Currently a MonoToStereoPanner but this could/should
    // be replaced with a better panner that can handle mono or stereo.
    Generator pan(ControlGenerator pan);

    //! Shortcut for wrapping with a Panner. Currently a MonoToStereoPanner but this could/should
    // be replaced with a better panner that can handle mono or stereo.
    Generator pan(float pan);
    
  protected:
  
    // keep track of whether an object has been added to the signal chain.
    // Once it has been added, you're no longer allowed to change the number of channels. Other restrictions may apply.
    void recordAdditionAsParameter(){ obj->hasBeenAdded = true; }
    
    // Subclass instances don't have access to protected members of parent class. This is a workaround.
    // Described here: http://stackoverflow.com/questions/3561648/why-does-c-not-allow-inherited-friendship/3562096#3562096
    void proxyRecordAdditionAsParameter(Generator& addition){ addition.recordAdditionAsParameter(); }

  };
  
  template<class GenType>
  class TemplatedGenerator : public Generator{
  protected:
    GenType* gen(){
      return static_cast<GenType*>(obj);
    }
  public:
    TemplatedGenerator() : Generator(new GenType) {}
  };
  
}

// Each generator should have three flavors of setter -- one that accepts a float, one that accepts a
// ControlGenerator, and one that accepts a Generator. This macro will automatically build those three
// setters

#include "FixedValue.h"

#define TONIC_MAKE_GEN_SETTERS(generatorClassName, methodNameInGenerator, methodNameInGenerator_) \
                                                                                        \
                                                                                        \
  generatorClassName& methodNameInGenerator(Generator arg){                             \
    this->gen()->methodNameInGenerator_(arg);                                           \
    Generator::proxyRecordAdditionAsParameter( arg );                                   \
    return static_cast<generatorClassName&>(*this);                                     \
  }                                                                                     \
                                                                                        \
  generatorClassName& methodNameInGenerator(float arg){                                 \
    return methodNameInGenerator( FixedValue(arg) );                                    \
  }                                                                                     \
                                                                                        \
  generatorClassName& methodNameInGenerator(ControlGenerator arg){                      \
    return methodNameInGenerator(  FixedValue().setValue(arg) );                        \
  }



#endif
