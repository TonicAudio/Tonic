//
//  Generator.h
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

#ifndef ___013_1_23_melody__Generator__
#define ___013_1_23_melody__Generator__

#include <iostream>
#include "TonicFrames.h"

namespace Tonic {

  namespace Tonic_{

    class Generator_{
      
    public:
      Generator_();
      virtual ~Generator_();
      virtual void tick( TonicFrames& frames) = 0;
      
      // mutex for swapping inputs, etc
      void lockMutex();
      void unlockMutex();
      
    protected:
        
      pthread_mutex_t genMutex_;

    };
    
    inline void Generator_::lockMutex(){
      pthread_mutex_lock(&genMutex_);
    }
    
    inline void Generator_::unlockMutex(){
      pthread_mutex_unlock(&genMutex_);
    }
    
    /////////////////////

    class PassThroughGenerator_ : public Tonic_::Generator_{
    public:
      void tick( TonicFrames& frames){
      
      }
    };

  }

  
  class Generator{
  protected:
    Tonic_::Generator_* mGen;
    int* pcount;
  public:
    Generator() : mGen( new Tonic_::PassThroughGenerator_() ) , pcount(new int(1)) {}
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
    
    void tick(TonicFrames& frames){
      mGen->tick(frames);
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
  generatorClassName& methodNameInGenerator(Generator arg){                                       \
    gen()->lockMutex();                                                                           \
    gen()->methodNameInGenerator_(arg);                                                           \
    gen()->unlockMutex();                                                                         \
        return *this;                                                                             \
  }                                                                                               \
                                                                                                  \
  generatorClassName& methodNameInGenerator(float arg){                                           \
    return methodNameInGenerator( FixedValue(arg) );                                              \
  }                                                                                               \
                                                                                                  \
  generatorClassName& methodNameInGenerator(ControlGenerator arg){                                \
    return methodNameInGenerator(  FixedValue().setValue(arg) );                                  \
  } 

// use this version for templated classes - performs necessary casting

#define createGeneratorSettersInTemplate(generatorClassName, generatorClassName_,       \
  methodNameInGenerator, methodNameInGenerator_)                                        \
                                                                                        \
                                                                                        \
  generatorClassName& methodNameInGenerator(Generator arg){                             \
    generatorClassName_ *gen = dynamic_cast<generatorClassName_*>(this->mGen);  \
    gen->lockMutex();                                                                   \
    gen->methodNameInGenerator_(arg);                                                   \
    gen->unlockMutex();                                                                 \
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



#endif /* defined(___013_1_23_melody__Generator__) */
