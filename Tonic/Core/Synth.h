//
//  Synth.h
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

#ifndef __Tonic__Synth__
#define __Tonic__Synth__

#include <map>
#include "Tonic.h"
#include "BufferFiller.h"

namespace Tonic{
  
  class Synth  : public BufferFiller{
    
  public:
    
    Synth();
    
    // It's quite conceivable that we'll want to move the messaging stuff up into Source
    ControlValue              addParameter(string name, float value=0);
    void                      setParameter(string name, float value=1);
    
  protected:
    
    std::map<string, ControlValue> parameters;
    
  };
  
  // ------------------------------
  //
  // Adding factory/registration for easy wrappers and instantiation
  //
  // http://stackoverflow.com/questions/582331/is-there-a-way-to-instantiate-objects-from-a-string-holding-their-class-name
  //
  // Concrete source subclasses should implement a static registration struct to be registered by default:
  //
  // In MySource.h:
  //
  // private:
  //  static SourceRegister<MySource> reg;
  //
  // In Source.cpp
  //  SourceRegister<MySource> MySource::reg("MySource");
  //
  // -----------------------------
  
  template<typename T> Synth * createSynth() { return new T; }
  
  struct SynthFactory {
    
    typedef std::map<std::string, Synth*(*)()> map_type;
    
    static Synth * createInstance(std::string const& s) {
      map_type::iterator it = getMap()->find(s);
      if(it == getMap()->end()){
        string synthsList = "";
        
        for(it = getMap()->begin(); it != getMap()->end(); it++){
          synthsList = synthsList + it->first + "\n";
        }
        
        error("Error creating sysnth. Synth named \"" + s + "\" not found. Existing registered synths are: \n" + synthsList);
        
        return 0;
      }
      return it->second();
    }
    
  protected:
    static map_type * getMap() {
      // never delete'ed. (exist until program termination)
      // because we can't guarantee correct destruction order
      if(!map) { map = new map_type; }
      return map;
    }
    
  private:
    static map_type * map;
  };
  
  template<typename T>
  struct SourceRegister : SynthFactory {
    SourceRegister(std::string const& s) {
      getMap()->insert(std::make_pair(s, &createSynth<T>));
    }
  };
  
}

// This macro will auto-register a synth type. Just add it below the Synth class definition.

#define registerSynth(SynthName)              \
                                              \
class SynthName ## _RegistrationPoint {       \
                                              \
protected:                                    \
  static SourceRegister<SynthName> reg;       \
                                              \
};                                            \
                                                                                \
SourceRegister<SynthName> SynthName ## _RegistrationPoint::reg(#SynthName);     \

#endif /* defined(__Tonic__Synth__) */
