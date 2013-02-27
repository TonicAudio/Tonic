//
//  Generator.h
//  Tonic
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

#ifndef __Tonic__Generator__
#define __Tonic__Generator__

#include <iostream>
#include "TonicFrames.h"

namespace Tonic {

  namespace Tonic_{

    class Generator_{
      
    public:
      Generator_();
      virtual ~Generator_();
      
      virtual void tick( TonicFrames& frames, const SynthesisContext_ &context );
      
      // override point for defining generator behavior
      // subclasses should implment to fill frames with new data
      virtual void computeSynthesisBlock( const SynthesisContext_ &context ) = 0;
      
      // mutex for swapping inputs, etc
      void lockMutex();
      void unlockMutex();
      
      
      // set stereo/mono - changes number of channels in synthesisBlock_
      void setIsStereo( bool stereo );
      inline bool isStereo(){ return stereo_; };
      
    protected:
      
      bool            stereo_;
      TonicFrames     synthesisBlock_;
      unsigned long   lastFrameIndex_;
      
      pthread_mutex_t genMutex_;

    };
    
    inline void Generator_::tick(TonicFrames &frames, const SynthesisContext_ &context ){
      
      // check context to see if we need new frames
      if (context.elapsedFrames == 0 || lastFrameIndex_ != context.elapsedFrames){
        computeSynthesisBlock(context);
        lastFrameIndex_ = context.elapsedFrames;
      }
      
      // copy synthesis block to frames passed in
      frames.copy(synthesisBlock_);
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
    
    inline bool isStereo(){
      return mGen->isStereo();
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


#define createControlGeneratorSetters(generatorClassName, methodNameInGenerator, methodNameInGenerator_)\
\
  generatorClassName& methodNameInGenerator(float arg){                                 \
    return methodNameInGenerator( ControlValue(arg) );                                  \
  }                                                                                     \
                                                                                        \
  generatorClassName& methodNameInGenerator(ControlGenerator arg){                      \
    gen()->methodNameInGenerator_(arg);                                                   \
    return static_cast<generatorClassName&>(*this);                                     \
  }



#endif /* defined(__Tonic__Generator__) */
