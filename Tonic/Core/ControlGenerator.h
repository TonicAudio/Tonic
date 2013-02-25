//
//  ControlGenerator.h
//  TonicDemo
//
//  Created by Morgan Packard on 2/12/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

/*
  Morgan 2012.02.12 Could probably link this to the class structure of regular generator and save some code, but I don't see any big winds from doing that, and just want to bang it out. Note, however, that this is very very similar to the code for the audio rate generator.

*/

#ifndef TonicDemo_ControlGenerator_h
#define TonicDemo_ControlGenerator_h

#include "Tonic.h"

namespace Tonic {

  namespace Tonic_{

    class ControlGenerator_{
      
    public:
      ControlGenerator_();
      virtual ~ControlGenerator_();
            
      // mutex for swapping inputs, etc
      void lockMutex();
      void unlockMutex();
      
      // Only override the below two functions if you need to customize reuse behavior
      virtual bool hasChanged(const SynthesisContext_ & context);
      virtual TonicFloat getValue(const SynthesisContext_ & context);
      
      // Used to peek at last value, for initializing other generators (see ramped() method for example)
      TonicFloat getLastValue() { return lastValue_; };
      
    protected:
      
      // Override these two functions to implement a new ControlGenerator
      virtual bool        computeHasChanged(const SynthesisContext_ & context) = 0;
      virtual TonicFloat  computeValue(const SynthesisContext_ & context) = 0;
      
      bool            lastHasChanged_;
      TonicFloat      lastValue_;
      unsigned long   lastFrameIndex_HasChanged_;
      unsigned long   lastFrameIndex_Value_;
      
      pthread_mutex_t genMutex_;

    };
    
    inline void ControlGenerator_::lockMutex(){
      pthread_mutex_lock(&genMutex_);
    }
    
    inline void ControlGenerator_::unlockMutex(){
      pthread_mutex_unlock(&genMutex_);
    }

  }

  // local declaration
  class RampedValue;

  class ControlGenerator{
  protected:
    Tonic_::ControlGenerator_* mGen;
    int* pcount;
  public:
    ControlGenerator() : mGen() , pcount(new int(1)) {}
    ControlGenerator(const ControlGenerator& r): mGen(r.mGen), pcount(r.pcount){(*pcount)++;}
    ControlGenerator& operator=(const ControlGenerator& r)
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
    
    ~ControlGenerator(){
      if(--(*pcount) == 0){
        delete mGen;
        delete pcount;
      }
    }
    
    virtual bool hasChanged(const Tonic_::SynthesisContext_ & context){
      return mGen->hasChanged(context);
    }
    
    virtual TonicFloat getValue(const Tonic_::SynthesisContext_ & context){
      return mGen->getValue(context);
    }
    
    // shortcut for creating ramped value
    RampedValue ramped(float lenMs = 50);
    
  };
  
  template<class GenType> class TemplatedControlGenerator : public ControlGenerator{
  protected:
    GenType* gen(){
      return static_cast<GenType*>(mGen);
    }
    
  public:
    TemplatedControlGenerator(){
      delete mGen;
      mGen = new GenType();
    }
    
  };
  
}

#endif
