//
//  ControlGenerator.h
//  TonicDemo
//
//  Created by Morgan Packard on 2/12/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//


#ifndef __Tonic_ControlGenerator__
#define __Tonic_ControlGenerator__

#include "TonicCore.h"

namespace Tonic {
  
  typedef enum {
    ControlGeneratorStatusHasNotChanged = 0,
    ControlGeneratorStatusHasChanged
    
  } ControlGeneratorStatus;
  
  struct ControlGeneratorOutput{
    TonicFloat value;
    ControlGeneratorStatus status;
    
    ControlGeneratorOutput() : value(0), status(ControlGeneratorStatusHasNotChanged) {};
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
      
      // Used to peek at last value, for initializing other generators (see ramped() method for example)
      virtual ControlGeneratorOutput peek() { return lastOutput_; };
      
    protected:
      
      // Override these two functions to implement a new ControlGenerator
      virtual ControlGeneratorStatus  computeStatus(const SynthesisContext_ & context) = 0;
      virtual TonicFloat              computeValue(const SynthesisContext_ & context) = 0;
      
      ControlGeneratorOutput  lastOutput_;
      unsigned long           lastFrameIndex_;
      
      pthread_mutex_t         genMutex_;

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
    
    ControlGeneratorOutput tick( const Tonic_::SynthesisContext_ & context ){
      return mGen->tick(context);
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
