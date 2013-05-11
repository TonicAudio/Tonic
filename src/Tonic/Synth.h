//
//  Synth.h
//  Tonic
//
//  Created by Morgan Packard on 1/23/13.
//
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__Synth__
#define __Tonic__Synth__

#include <map>
#include <float.h>
#include "Tonic.h"
#include "BufferFiller.h"

namespace Tonic{
  
  class Synth  : public BufferFiller{
    
  public:
    
    typedef enum{
      
      SynthParameterTypeContinuous = 0,        
      SynthParameterTypeToggle,
      SynthParameterTypeMomentary,
      SynthParameterTypeTrigger
      
    } SynthParameterType;
    
    
    struct SynthParameter{
      string              name;
      string              displayName;
      ControlValue        value;
      SynthParameterType  type;
      float               min;
      float               max;
      
      SynthParameter();
    };
    
    Synth();
    
    // It's quite conceivable that we'll want to move the messaging stuff up into Source
    ControlValue  & addParameter(string name, float value=0, float min=0, float max=1.f);
    ControlValue  & addParameter(string name, string displayName, SynthParameterType type, float value=0, float min=0, float max=1.f);
    
    void            setParameter(string name, float value=1);
    
    vector<SynthParameter> getParameters();
    
  protected:

    // set to true in constructor to clamp incoming parameters to defined min/max
    bool  clampsParameters_; 
    std::map<string, SynthParameter> parameters_;
    
  };
  
  // ------------------------------
  //
  // Adding factory/registration for easy wrappers and instantiation
  //
  // http://stackoverflow.com/questions/582331/is-there-a-way-to-instantiate-objects-from-a-string-holding-their-class-name
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
        
        error("Error creating synth. Synth named \"" + s + "\" not found. Existing registered synths are: \n" + synthsList);
        
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
