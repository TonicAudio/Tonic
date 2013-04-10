//
//  Generator.h
//  Tonic
//
//  Created by Morgan Packard on 1/23/13.
//
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__Generator__
#define __Tonic__Generator__

#include "TonicFrames.h"

namespace Tonic {

  namespace Tonic_{

    class Generator_{
      
    public:
      Generator_();
      virtual ~Generator_();
      
      virtual void tick( TonicFrames& frames, const SynthesisContext_ &context );
      
      // mutex for swapping inputs, etc
      void lockMutex();
      void unlockMutex();
      
      bool isStereoOutput(){ return isStereoOutput_; };
      
      // set stereo/mono - changes number of channels in synthesisBlock_
      // subclasses should call in constructor to determine channel output
      virtual void setIsStereoOutput( bool stereo );
      
    protected:
      
      // override point for defining generator behavior
      // subclasses should implment to fill frames with new data
      virtual void computeSynthesisBlock( const SynthesisContext_ &context ) = 0;

      
      bool            isStereoOutput_;
      TonicFrames     synthesisBlock_;
      unsigned long   lastFrameIndex_;
      
      pthread_mutex_t genMutex_;

    };
    
    inline void Generator_::tick(TonicFrames &frames, const SynthesisContext_ &context ){
      
      // check context to see if we need new frames
      if (context.elapsedFrames == 0 || lastFrameIndex_ != context.elapsedFrames){
        lockMutex();
        computeSynthesisBlock(context);
        unlockMutex();
        lastFrameIndex_ = context.elapsedFrames;
      }
    
      // copy synthesis block to frames passed in
      frames.copy(synthesisBlock_);
      
      #ifdef TONIC_DEBUG
      if(frames(0,0) != frames(0,0)){
        Tonic::error("Generator_::tick NaN detected.");
      }
      #endif
      
    }
    
    inline void Generator_::lockMutex(){
      pthread_mutex_lock(&genMutex_);
    }
    
    inline void Generator_::unlockMutex(){
      pthread_mutex_unlock(&genMutex_);
    }
    
    /////////////////////

    class PassThroughGenerator_ : public Tonic_::Generator_{
    public:
      void computeSynthesisBlock( const SynthesisContext_ &context ) {};
    };

  }

  
  class Generator{
  protected:
    Tonic_::Generator_* mGen;
    int* pcount;
  public:
    Generator() : mGen( new Tonic_::PassThroughGenerator_() ), pcount(new int(1)) {}
    Generator(const Generator& r): mGen(r.mGen), pcount(r.pcount){(*pcount)++;}
    Generator& operator=(const Generator& r)
    {
      if(mGen == r.mGen) return *this;
      if(--(*pcount) == 0){
        delete mGen;
        delete pcount;
      }
      mGen = r.mGen;
      pcount = r.pcount;
      (*pcount)++;
      return *this;
    }
    
    ~Generator(){
      if(--(*pcount) == 0){
        delete mGen;
        delete pcount;
      }
    }
    
    bool operator==(const Generator& r){
      return mGen == r.mGen;
    }
    
    inline bool isStereoOutput(){
      return mGen->isStereoOutput();
    }
    
    virtual void tick(TonicFrames& frames, const Tonic_::SynthesisContext_ & context){
      mGen->tick(frames, context);
    }

  };
  
  template<class GenType> class TemplatedGenerator : public Generator{
  protected:
    GenType* gen(){
      return static_cast<GenType*>(mGen);
    }
  public:
    TemplatedGenerator(){
      delete mGen;
      mGen = new GenType();
    }
  };
  
  class PassThroughGenerator : public TemplatedGenerator<Tonic_::PassThroughGenerator_>{
  public:
  
  };
  
}

// Each generator should have three flavors of setter -- one that accepts a float, one that accepts a
// ControlGenerator, and one that accepts a Generator. This macro will automatically build those three
// setters

#include "FixedValue.h"

#define createGeneratorSetters(generatorClassName, methodNameInGenerator, methodNameInGenerator_) \
                                                                                        \
                                                                                        \
  generatorClassName& methodNameInGenerator(Generator arg){                             \
    this->gen()->lockMutex();                                                           \
    this->gen()->methodNameInGenerator_(arg);                                           \
    this->gen()->unlockMutex();                                                         \
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



#endif /* defined(__Tonic__Generator__) */
