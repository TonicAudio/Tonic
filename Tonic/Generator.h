//
//  Generator.h
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/23/13.
//
//

#ifndef ___013_1_23_melody__Generator__
#define ___013_1_23_melody__Generator__

#include <iostream>
#include "TonicFrames.h"

namespace Tonic {


class Generator_{
  
public:
  Generator_();
  virtual ~Generator_();
  virtual void tick( TonicFrames& frames) = 0;
protected:

};

/////////////////////

  class PassThroughGenerator_ : public Generator_{
  public:
    void tick( TonicFrames& frames){
    
    }
  };
  
  class Generator{
  protected:
    Generator_* mGen;
    int* pcount;
  public:
    Generator() : mGen( new PassThroughGenerator_() ) , pcount(new int(1)) {}
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
    void tick(TonicFrames& frames){
      mGen->tick(frames);
    }
    ~Generator(){
      if(--(*pcount) == 0){
        delete mGen;
        delete pcount;
      }
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
  
  class PassThroughGenerator : public TemplatedGenerator<PassThroughGenerator_>{
  public:
  
  };
  

  

  
  


  
}

#endif /* defined(___013_1_23_melody__Generator__) */
